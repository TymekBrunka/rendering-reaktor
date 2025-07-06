#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
use std::f32::consts::PI;

use egui::{Layout, Pos2, Rect, Response};
// hide console window on Windows in release
// use egui_miniquad as egui_mq;
use miniquad as mq;
use miniquad::*;

use glam::{Mat4, Vec2, Vec3, vec2, vec3};

mod cube;
use cube::*;

#[repr(C)]
#[derive(Debug)]
struct Vertex {
    pos: [f32; 3],
    color: [f32; 4],
}

struct Camera {
    projection: Mat4,
    position: Vec3,
    orientation: Vec2,
}

struct MyMiniquadApp {
    pipeline: Pipeline,
    bindings: Bindings,
    ctx: Box<dyn RenderingBackend>,

    egui_mq: egui_miniquad::EguiMq,
    camera: Camera,
    ic: u16,

    prev_pos: Vec2,
}

impl MyMiniquadApp {
    fn new() -> Self {
        let mut ctx: Box<dyn RenderingBackend> = window::new_rendering_backend();

        let (width, height) = window::screen_size();
        let projection =
            Mat4::perspective_rh_gl(120.0f32.to_radians(), width / height, 0.001, 100.0);
        let camera: Camera = Camera {
            projection: projection,
            position: vec3(0., 0., 0.),
            orientation: vec2(0., 0.),
        };

        let mut ic: u16 = 0;
        let mut vc: u16 = 0;
        let mut v: Vec<Vertex> = Vec::new();
        let mut i: Vec<u16> = Vec::new();

        gen_cube!(v, i, ic, vc, -1., -1., -1., 1., 1., 1.);
        gen_cube!(v, i, ic, vc, 0., 0., -1., 1., 1., 1.);

        println!("{:#?}", v);

        #[rustfmt::skip]
        // let vertices: &[Vertex] = &[
        //     Vertex { pos : [ -1.0, -1.0, 1.0 ], color: [1., 0., 0., 1.] },
        //     Vertex { pos : [  1.0, -1.0, 1.0 ], color: [0., 1., 0., 1.] },
        //     Vertex { pos : [  -1.0, 1.0, 1.0 ], color: [0., 0., 1., 1.] },
        //     Vertex { pos : [  1.0,  1.0, 1.0 ], color: [0., 0., 1., 1.] },
        // ];

        let vertices: &[Vertex] = v.as_slice();

        let vertex_buffer = ctx.new_buffer(
            BufferType::VertexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(&vertices),
        );

        // let indices: &[u16] = &[0, 1, 2, 1, 2, 3];

        let indices: &[u16] = i.as_slice();

        let index_buffer = ctx.new_buffer(
            BufferType::IndexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(&indices),
        );

        let bindings = Bindings {
            vertex_buffers: vec![vertex_buffer],
            index_buffer: index_buffer,
            images: vec![],
        };

        let shader = ctx
            .new_shader(
                ShaderSource::Glsl {
                    vertex: shader::VERTEX,
                    fragment: shader::FRAGMENT,
                },
                shader::meta(),
            )
            .unwrap();

        let pipeline = ctx.new_pipeline(
            &[
                BufferLayout::default(),
                BufferLayout {
                    step_func: VertexStep::PerInstance,
                    ..Default::default()
                },
            ],
            &[
                VertexAttribute::new("in_pos", VertexFormat::Float3),
                VertexAttribute::new("in_color", VertexFormat::Float4),
            ],
            shader,
            PipelineParams {
                cull_face: CullFace::Nothing,
                ..Default::default()
            },
        );

        MyMiniquadApp {
            pipeline,
            bindings,

            egui_mq: egui_miniquad::EguiMq::new(&mut *ctx),
            ctx,
            camera,
            ic,

            prev_pos: vec2(0., 0.),
        }
    }
}

mod shader {
    use miniquad::*;

    // pub const VERTEX: &str = r#"#version 100
    // attribute vec3 in_pos;
    // attribute vec4 in_color;
    //
    // varying lowp vec4 color;
    //
    // void main() {
    //     gl_Position = vec4(in_pos, 1);
    //     color = in_color;
    // }"#;

    pub const VERTEX: &str = r#"#version 100
    attribute vec3 in_pos;
    attribute vec4 in_color;

    varying lowp vec4 color;

    uniform mat4 mpv;

    void main()
    {
        gl_Position = mpv * vec4(in_pos, 1.0);
        color = in_color;
    }"#;

    pub const FRAGMENT: &str = r#"#version 100
    varying lowp vec4 color;

    void main() {
        gl_FragColor = color;
    }"#;

    pub fn meta() -> ShaderMeta {
        ShaderMeta {
            images: vec![],
            uniforms: UniformBlockLayout {
                uniforms: vec![UniformDesc::new("mpv", UniformType::Mat4)],
            },
        }
    }

    #[repr(C)]
    pub struct Uniforms {
        pub mpv: glam::Mat4,
    }
}

impl mq::EventHandler for MyMiniquadApp {
    fn update(&mut self) {}

    fn draw(&mut self) {
        let view = Mat4::look_at_rh(self.camera.position, vec3(0., 0., 0.), vec3(0., 1., 0.));
        let vs_params = shader::Uniforms {
            mpv: self.camera.projection
                * view
                * (Mat4::from_rotation_y(self.camera.orientation.x)
                    * Mat4::from_rotation_x(self.camera.orientation.y)),
        };

        self.ctx
            .begin_default_pass(mq::PassAction::clear_color(0.0, 0.0, 0.0, 1.0));
        //gl
        self.ctx.apply_pipeline(&self.pipeline);
        self.ctx.apply_bindings(&self.bindings);
        self.ctx.apply_uniforms(UniformsSource::table(&vs_params));
        self.ctx.draw(0, self.ic.into(), 1);
        //end gl
        self.ctx.end_render_pass();

        self.egui_mq.run(&mut *self.ctx, |_mq_ctx, egui_ctx| {
            // egui::Window::new("Egui Window").show(egui_ctx, |ui| {
            //     ui.heading("Hello World!");
            // });
            egui::SidePanel::right("my_right_panel").show(egui_ctx, |ui| {
                ui.horizontal(|ui| {
                    ui.heading(format!(
                        "rotat: {}{}",
                        self.camera.orientation.x, self.camera.orientation.y
                    ))
                })
            });
        });

        // Draw things behind egui here

        self.egui_mq.draw(&mut *self.ctx);

        // Draw things in front of egui here

        self.ctx.commit_frame();
    }

    fn mouse_motion_event(&mut self, x: f32, y: f32) {
        self.egui_mq.mouse_motion_event(x, y);
        let vek = vec2(x, y);
        self.camera.orientation += ((vek - self.prev_pos) / -180.) * PI;
        self.prev_pos = vek;
    }

    fn mouse_wheel_event(&mut self, dx: f32, dy: f32) {
        self.egui_mq.mouse_wheel_event(dx, dy);
    }

    fn mouse_button_down_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_down_event(mb, x, y);
    }

    fn mouse_button_up_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_up_event(mb, x, y);
    }

    fn char_event(&mut self, character: char, _keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.char_event(character);
    }

    fn key_down_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.key_down_event(keycode, keymods);
        match keycode {
            mq::KeyCode::W => {
                self.camera.position.z -= 1.;
            }
            mq::KeyCode::S => {
                self.camera.position.z += 1.;
            }
            _ => return,
        }
    }

    fn key_up_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods) {
        self.egui_mq.key_up_event(keycode, keymods);
    }
}

fn main() {
    miniquad::start(mq::conf::Conf::default(), || Box::new(MyMiniquadApp::new()));
}
