#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
use std::f32::consts::PI;
// hide console window on Windows in release
// use egui_miniquad as egui_mq;
use miniquad as mq;
use miniquad::*;

use glam::{Mat4, Vec2, Vec3, vec2, vec3};

use std::time::Instant;

mod util;
use util::*;

#[repr(C)]
#[derive(Debug)]
struct Vertex {
    pos: [f32; 3],
    typ: f32,
    rat: f32
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
    last_frame_t: Instant,
    holding_mouse: bool,
    kb_motion: Vec2,

    screen_size: Vec2,
}

impl MyMiniquadApp {
    fn new() -> Self {
        let mut ctx: Box<dyn RenderingBackend> = window::new_rendering_backend();

        let (width, height) = window::screen_size();
        let screen_size = vec2(width, height);
        let projection =
            Mat4::perspective_rh_gl(120.0f32.to_radians(), width / height, 0.001, 100000.0);
        let camera: Camera = Camera {
            projection: projection,
            position: vec3(0., 0., -30.),
            orientation: vec2(0., 0.),
        };

        let mut ic: u16 = 0;
        let mut vc: u16 = 0;
        let mut v: Vec<Vertex> = Vec::new();
        let mut i: Vec<u16> = Vec::new();

        //
        //
        // ------- [ gen geometry ] --------
        //
        //

        gen_cube!(v, i, ic, vc, -20., -6., -30., 40., 6., 60.);
        gen_point!(v, i, ic, vc, 0., 0., 0.);

        //
        //

        // println!("{:#?}", v);
        println!("{:b}", 0x00000001 as u32);

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
                VertexAttribute::new("typ", VertexFormat::Float1),
                VertexAttribute::new("rat", VertexFormat::Float1),
            ],
            shader,
            PipelineParams {
                cull_face: CullFace::Back,
                depth_test: Comparison::LessOrEqual,
                depth_write: true,

                color_blend: Some(BlendState::new(
                    Equation::Add,
                    BlendFactor::Value(BlendValue::SourceAlpha),
                    BlendFactor::OneMinusValue(BlendValue::SourceAlpha))
                ),
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
            last_frame_t: Instant::now(),
            holding_mouse: false,
            kb_motion: vec2(0.0, 0.0),

            screen_size
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
    attribute float typ;
    attribute float rat;

    varying lowp vec4 color;
    varying mediump float typp;
    varying mediump float ratt;
    varying lowp vec2 point_cord;

    uniform mediump mat4 mpv;
    uniform mediump vec2 unit_size;

    void main()
    {
        typp = typ;
        gl_Position = mpv * vec4(in_pos * vec3(1.0, 1.0, -1.0), 1.0);
        if (typ == 1.0) {
            color = mix(
                vec4(0.1, 0.0, 0.05, 1.0),
                mix(vec4(0.3, 0.6, 0.5, 1.0), vec4(0.2, 0.4, 0.6, 1.0), rat),
                1.0 / clamp(gl_Position.z, 10.0, 10000.0) * 10.0
                // typ
            );
        } else if (typ == 2.0) {
            gl_Position.xyz /= gl_Position.w;
            gl_Position.z = -clamp(gl_Position.z, -1.0, 5.0);
            // gl_Position.xyz /= gl_Position.w;
            gl_Position.w = 1.0;
            if (rat == 0.0) {
                gl_Position.xy += vec2(-0.01, 0.02);
                point_cord = vec2(-1.0, 2.0);
            } else if (rat == 1.0) {
                gl_Position.xy += vec2(-0.01, -0.01);
                point_cord = vec2(-1.0, -1.0);
            } else {
                gl_Position.xy += vec2(0.02, -0.01);
                point_cord = vec2(2.0, -1.0);
            }
        }
    }"#;

    pub const FRAGMENT: &str = r#"#version 100
    varying lowp vec4 color;
    varying mediump float typp;
    varying mediump float ratt;
    varying lowp vec2 point_cord;

    lowp float a;

    uniform mediump mat4 mpv;

    void main() {
        gl_FragColor = color;
        if (typp == 2.0) {
            a = 1.0 - length(point_cord);
            gl_FragColor = mix(vec4(0.0), vec4(1.0), a * 2.0);
            // gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
        }
    }"#;

    pub fn meta() -> ShaderMeta {
        ShaderMeta {
            images: vec![],
            uniforms: UniformBlockLayout {
                uniforms: vec![UniformDesc::new("mpv", UniformType::Mat4), UniformDesc::new("unit_size", UniformType::Float2)],
            },
        }
    }

    #[repr(C)]
    pub struct Uniforms {
        pub mpv: glam::Mat4,
        pub unit_size: glam::Vec2
    }
}

impl mq::EventHandler for MyMiniquadApp {
    fn update(&mut self) {}

    fn draw(&mut self) {
        let rot = self.camera.orientation;
        let forward: Vec3 = vec3(
            rot.x.sin() * rot.y.cos(),
            rot.y.sin(),
            rot.x.cos() * rot.y.cos(),
        );

        let right = Vec3::cross(forward, vec3(0., 1., 0.));
        let up = Vec3::cross(right, forward);

        //quick cam movement
        let now = Instant::now();
        let deltatime: f32 = now.duration_since(self.last_frame_t).as_secs_f32();
        self.last_frame_t = now;
        self.camera.position = self.camera.position + 20. * deltatime * ( forward * self.kb_motion.y + right * self.kb_motion.x );

        let view = Mat4::look_at_rh(self.camera.position, self.camera.position + forward, up);
        let vs_params = shader::Uniforms {
            mpv: self.camera.projection * view,
                // * (Mat4::from_rotation_y(self.camera.orientation.x)
                //     * Mat4::from_rotation_x(self.camera.orientation.y)),
            unit_size: vec2(1., 1.) / self.screen_size
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
                ui.vertical(|ui| {
                    ui.horizontal(|ui| {
                        ui.heading(format!(
                            "rotat: {} {}",
                            self.camera.orientation.x, self.camera.orientation.y
                        ))
                    });
                    ui.horizontal(|ui| {
                        ui.heading(format!(
                            "forward: {}",
                            forward
                        ))
                    });
                    ui.horizontal(|ui| {
                        ui.heading(format!(
                            "up: {}",
                            up
                        ))
                    });
                    ui.horizontal(|ui| {
                        ui.heading(format!(
                            "kb_motion: {}",
                            self.kb_motion
                        ))
                    });
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
        if self.holding_mouse {
            self.camera.orientation += ((vek - self.prev_pos) / -180.) * PI;
            self.camera.orientation = vec2(self.camera.orientation.x % (2. * PI), self.camera.orientation.y % (2. * PI));
        }
        self.prev_pos = vek;
    }

    fn mouse_wheel_event(&mut self, dx: f32, dy: f32) {
        self.egui_mq.mouse_wheel_event(dx, dy);
    }

    fn mouse_button_down_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_down_event(mb, x, y);
        if mb == mq::MouseButton::Right {
            self.holding_mouse = true;
        }
    }

    fn mouse_button_up_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_up_event(mb, x, y);
        if mb == mq::MouseButton::Right {
            self.holding_mouse = false;
        }
    }

    fn char_event(&mut self, character: char, _keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.char_event(character);
    }

    fn key_down_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.key_down_event(keycode, keymods);
        match keycode {
            mq::KeyCode::W => {
                self.kb_motion.y += 1.;
            }
            mq::KeyCode::S => {
                self.kb_motion.y -= 1.;
            }
            mq::KeyCode::A => {
                self.kb_motion.x -= 1.;
            }
            mq::KeyCode::D => {
                self.kb_motion.x += 1.;
            }
            _ => return,
        }
        self.kb_motion.x = self.kb_motion.x.clamp(-1., 1.);
        self.kb_motion.y = self.kb_motion.y.clamp(-1., 1.);
    }

    fn key_up_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods) {
        self.egui_mq.key_up_event(keycode, keymods);
        match keycode {
            mq::KeyCode::W => {
                self.kb_motion.y -= 1.;
            }
            mq::KeyCode::S => {
                self.kb_motion.y += 1.;
            }
            mq::KeyCode::A => {
                self.kb_motion.x += 1.;
            }
            mq::KeyCode::D => {
                self.kb_motion.x -= 1.;
            }
            _ => return,
        }
        self.kb_motion.x = self.kb_motion.x.clamp(-1., 1.);
        self.kb_motion.y = self.kb_motion.y.clamp(-1., 1.);
    }
}

fn main() {
    miniquad::start(mq::conf::Conf::default(), || Box::new(MyMiniquadApp::new()));
}
