#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

mod glium_util;
mod util;
mod types;

use std::time::Instant;

use egui_glium::EguiGlium;
use glam::{vec3, vec2, Vec3, Mat4};
use glium::{uniform, uniforms};
use glium::{
    vertex::VertexBuffer,
    IndexBuffer,
    Display,
    program,
    Program,
    Surface,
    index::PrimitiveType,
    texture::Texture2d
};
use glium::glutin::{
    self,
    surface::WindowSurface,
    display::GetGlDisplay,
};
use winit::event::MouseButton;
use winit::keyboard::SmolStr;
use winit::{
    application::ApplicationHandler,
    event::{StartCause, WindowEvent, KeyEvent, ElementState},
    event_loop::{ActiveEventLoop, EventLoop},
    window::{Window, WindowId},
    keyboard::{Key, NamedKey},
};

use glium_util::*;
use util::*;
use types::*;

struct Application {
    verticies_3d: VertexBuffer<Vertex3D>,
    indiecies_3d: IndexBuffer<u16>,
    shader_3d: Program,

    // verticies_ui: VertexBuffer<VertexUi>,
    // indiecies_ui: IndexBuffer<u16>,
    // shader_ui: Program,

    // composite_shader: Program,
    ascii_strip: Texture2d,
    camera: Camera,
    input: InputData
}

fn make_camera_matrix_and_move_camera(app: &mut Application) -> Mat4 {
    let rot = app.camera.orientation;
    let forward = vec3(
        rot.x.sin() * rot.y.cos(),
        rot.y.sin(),
        rot.x.cos() * rot.y.cos(),
    );

    let right = Vec3::cross(forward, vec3(0., 1., 0.));
    let up = Vec3::cross(right, forward);

    //quick cam movement
    let right_movement = vec3(
        rot.x.cos(),
        rot.y.sin(),
        -rot.x.sin(),
    );

    let now = Instant::now();
    let deltatime: f32 = now.duration_since(app.camera.last_frame_t).as_secs_f32();
    app.camera.last_frame_t = now;
    app.camera.position = app.camera.position + 20. * deltatime * ( forward * app.input.motion.y + right_movement * app.input.motion.x );

    let view = Mat4::look_at_rh(app.camera.position, app.camera.position + forward, up);
    return app.camera.projection * view;
}

trait ConvertMatrixToArray<M, A> {
    fn convert(self) -> A;
}

impl ConvertMatrixToArray<Mat4, [[f32;4];4]> for Mat4 {
    fn convert(self: Mat4) -> [[f32;4];4] {
        return [
            [ self.x_axis.x, self.x_axis.y, self.x_axis.z, self.x_axis.z ],
            [ self.y_axis.x, self.y_axis.y, self.y_axis.z, self.y_axis.z ],
            [ self.z_axis.x, self.z_axis.y, self.z_axis.z, self.z_axis.z ],
            [ self.w_axis.x, self.w_axis.y, self.w_axis.z, self.w_axis.z ],
        ]
    }
}

impl ApplicationContext for Application {
    const WINDOW_TITLE:&'static str = "Wizulizator stanu reaktorów biologicznych w 3D";
    fn new(display: &Display<WindowSurface>) -> Self {
        //world---
        
        let mut v3d: Vec<Vertex3D> = Vec::new();
        let mut i3d: Vec<u16> = Vec::new();
        let mut v3d_c = 0u16;
        let mut i3d_c = 0u16;

        gen_cube!(v3d, i3d, i3d_c, v3d_c, -20., -3., -30., 40., 6., 60.);

        //--------
        
        let projection = Mat4::perspective_rh_gl(120.0f32.to_radians(), 800./500., 0.001, 100000.0);
        let camera = Camera {
            position: vec3(0.0, 0.0, -32.0),
            orientation: vec2(0.0, 0.0),
            projection,

            last_frame_t: Instant::now(),
            screen_size: (800, 500),
            unit_size: vec2(1./800., 1./500.)
        };

        let input = InputData {
            motion: vec2(0.0, 0.0),
            holding_rmb: false,
        };

        let (header, image_data) = png_decoder::decode(include_bytes!("strip.png")).unwrap();
        let image = glium::texture::RawImage2d::from_raw_rgba_reversed(&image_data, (header.width, header.height));
        let ascii_strip = glium::texture::Texture2d::new(display, image).unwrap();

        //opengl---

        let verticies_3d = VertexBuffer::new(display, v3d.as_slice()).unwrap();
        let indiecies_3d = IndexBuffer::new(display, PrimitiveType::TriangleStrip, i3d.as_slice()).unwrap();

        let shader_3d = program!(display,
            330 => {
                vertex: include_str!("shaders/3d_vertex.glsl"),
                fragment: include_str!("shaders/3d_fragment.glsl"),
            }
        ).unwrap();

        Self {
            verticies_3d,
            indiecies_3d,
            shader_3d,

            ascii_strip,
            camera,
            input
        }
    }
    fn draw_frame(&mut self, display: &Display<WindowSurface>, _egui: &EguiGlium) {
        let mut frame = display.draw();

        let uniforms = uniform! {
            pv_matrix: make_camera_matrix_and_move_camera(self).convert()
        };

        frame.clear_color(0.05, 0.0, 0.1, 1.0);
        frame
            .draw(
                &self.verticies_3d,
                &self.indiecies_3d,
                &self.shader_3d,
                &uniforms,
                &Default::default(),
            )
            .unwrap();
        frame.finish().unwrap();
    }
    fn resize_window(&mut self, new_size: (u32, u32)) {
        let projection = Mat4::perspective_rh_gl(120.0f32.to_radians(), new_size.0 as f32 / new_size.1 as f32, 0.001, 100000.0);
        self.camera.projection = projection;
        self.camera.unit_size = vec2(1. / new_size.0 as f32, 1. / new_size.1 as f32);
    }
    fn update(&mut self) { }
    fn handle_window_event(&mut self, event: &glium::winit::event::WindowEvent, _window: &glium::winit::window::Window) {
        match &event {
            WindowEvent::KeyboardInput { device_id, event, is_synthetic } => {
                if event.state.is_pressed() {
                    if event.logical_key == "w" {
                        self.input.motion.y +=  1.;
                    } else if event.logical_key == "s" {
                        self.input.motion.y += -1.;
                    } else if event.logical_key == "a" {
                        self.input.motion.x += -1.;
                    } else if event.logical_key == "d" {
                        self.input.motion.x +=  1.;
                    }
                } else if !event.state.is_pressed() {
                    if event.logical_key == "w" {
                        self.input.motion.y -=  1.;
                    } else if event.logical_key == "s" {
                        self.input.motion.y -= -1.;
                    } else if event.logical_key == "a" {
                        self.input.motion.x -= -1.;
                    } else if event.logical_key == "d" {
                        self.input.motion.x -=  1.;
                    }
                }
                self.input.motion.x = self.input.motion.x.clamp(-1., 1.);
                self.input.motion.y = self.input.motion.y.clamp(-1., 1.);
            }

            WindowEvent::MouseInput { device_id, state, button } => {
                if button == MouseButton::Right {
                    if state == ElementState::Pressed {
                        self.input.holding_rmb = true;
                    } else {
                        self.input.holding_rmb = false;
                    }
                }
            }

            _ => {}
        }
    }
}

fn main() {
    State::<Application>::run_loop();
}
