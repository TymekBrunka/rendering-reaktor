#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
use std::f32::consts::PI;
// hide console window on Windows in release
// use egui_miniquad as egui_mq;
use miniquad as mq;
use miniquad::*;

use rfd::FileDialog;
use calamine::{Reader, open_workbook, Xlsx, DataType};

//IOEXCEL DESER
#[derive(Debug)]
struct Row {
    // #[column(name = "I21_RB1KO_PO4.Wartosc")]
    rb1ko_po4: String,
    // #[column(name = "I21_RB1KO_NH4.Wartosc")]
    rb1ko_nh4: String,

}

use glam::{Mat4, Vec2, Vec3, vec2, vec3};

use std::time::Instant;

mod util;
mod types;
// mod gl_version_fix;
// use gl_version_fix::new_gl_context;
use util::*;
use types::*;

struct MyMiniquadApp {
    display_pipeline: Pipeline,
    display_bindings: Bindings,

    deffered_3d_pipeline: Pipeline,
    deffered_3d_bindings: Bindings,
    deffered_3d_pass: RenderPass,

    deffered_ui_pipeline: Pipeline,
    deffered_ui_bindings: Bindings,
    deffered_ui_pass: RenderPass,

    ctx: Box<GlContext>,

    egui_mq: egui_miniquad::EguiMq,
    camera: Camera,
    input: InputData,

    indices_3d_count: u16,
    indices_ui_count: u16,
    tx_id: TextureId
}

impl MyMiniquadApp {
    fn new() -> Self {
        //gen ctx
        let gl_ctx = GlContext::new();
        let mut ctx: Box<GlContext> = Box::new(gl_ctx);

        //load ascii strip
        let (header, image_data) = png_decoder::decode(include_bytes!("strip.png")).unwrap();
        let tx_id = ctx.new_texture_from_rgba8(header.width as u16, header.height as u16, image_data.as_slice());

        //make perspective matrix
        let (width, height) = window::screen_size();
        let projection =
            Mat4::perspective_rh_gl(120.0f32.to_radians(), width / height, 0.001, 100000.0);

        let camera = Camera {
            projection: projection,
            position: vec3(0., 0., -30.),
            orientation: vec2(0., 0.),

            last_frame_t: Instant::now(),
            screen_size: vec2(width, height),
            unit_size: vec2(1./width, 1./height)
        };

        let input = InputData {
            motion: vec2(0.0, 0.0),
            holding_rmb: false,
            last_mouse_position: vec2(0.0, 0.0)
        };

        let mut ic3d: u16 = 0;
        let mut vc3d: u16 = 0;
        let mut v3d: Vec<Vertex3D> = Vec::new();
        let mut i3d: Vec<u16> = Vec::new();

        let mut icui: u16 = 0;
        let mut vcui: u16 = 0;
        let mut vui: Vec<VertexUi> = Vec::new();
        let mut iui: Vec<u16> = Vec::new();

        //
        //
        // ------- [ gen geometry ] --------
        //
        //

        gen_cube!(v3d, i3d, ic3d, vc3d, -20., -3., -30.,  40., 6., 60., [0.3, 0.6, 0.5], [0.2, 0.4, 0.6]);
        gen_cube!(v3d, i3d, ic3d, vc3d, -20., -3., -30.,  40., 6., 9., [0.2, 0.6, 0.3], [0.5, 0.4, 0.2]);
        gen_cube!(v3d, i3d, ic3d, vc3d, -20., -3., -21.,  40., 6., 9., [0.4, 0.8, 0.3], [0.2, 0.8, 0.5]);
        gen_cube!(v3d, i3d, ic3d, vc3d, -20., -3., -12.,  20., 6., 9., [1.0, 0.6, 0.5], [1.0, 0.4, 0.6]);
        gen_cube!(v3d, i3d, ic3d, vc3d,   0., -3., -12.,  20., 6., 9., [0.1, 0.3, 0.8], [0.4, 0.2, 0.8]);

        //
        //


        // -- deffered - ui --
        let ui_rendering_texture = ctx.new_render_texture(TextureParams {
            width: width as u32,
            height: height as u32,
            format: TextureFormat::RGBA8,
            ..Default::default()
        });

        let deffered_ui_pass = ctx.new_render_pass(ui_rendering_texture, None);

        let vertex_ui_buffer = ctx.new_buffer(
            BufferType::VertexBuffer,
            BufferUsage::Dynamic,
            BufferSource::slice(vui.as_slice()),
        );

        let index_ui_buffer = ctx.new_buffer(
            BufferType::IndexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(iui.as_slice()),
        );

        let deffered_ui_bindings = Bindings {
            vertex_buffers: vec![vertex_ui_buffer],
            index_buffer: index_ui_buffer,
            images: vec![tx_id],
        };

        let shader_ui = ctx
            .new_shader(
                ShaderSource::Glsl {
                    vertex: shaderUi::VERTEX,
                    fragment: shaderUi::FRAGMENT,
                },
                shaderUi::meta(),
            )
            .unwrap();

        let deffered_ui_pipeline = ctx.new_pipeline(
            &[
                BufferLayout::default(),
                // BufferLayout {
                //     step_func: VertexStep::PerInstance,
                //     ..Default::default()
                // },
            ],
            &[
                VertexAttribute::new("position", VertexFormat::Float3),
                VertexAttribute::new("element_type", VertexFormat::Float1),
                VertexAttribute::new("vertex_type", VertexFormat::Float1),
                VertexAttribute::new("char_offset", VertexFormat::Float1),
                VertexAttribute::new("char_index", VertexFormat::Float1)
            ],
            shader_ui,
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

        // -- defered - 3d --
        let rendering_3d_texture = ctx.new_render_texture(TextureParams {
            width: width as u32,
            height: height as u32,
            format: TextureFormat::RGBA8,
            ..Default::default()
        });

        let deffered_3d_pass = ctx.new_render_pass(rendering_3d_texture, None);

        let vertex_3d_buffer = ctx.new_buffer(
            BufferType::VertexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(v3d.as_slice()),
        );

        let index_3d_buffer = ctx.new_buffer(
            BufferType::IndexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(i3d.as_slice()),
        );

        let deffered_3d_bindings = Bindings {
            vertex_buffers: vec![vertex_3d_buffer],
            index_buffer: index_3d_buffer,
            images: vec![],
        };

        let shader_3d = ctx
            .new_shader(
                ShaderSource::Glsl {
                    vertex: shader3D::VERTEX,
                    fragment: shader3D::FRAGMENT,
                },
                shader3D::meta(),
            )
            .unwrap();

        let deffered_3d_pipeline = ctx.new_pipeline(
            &[
                BufferLayout::default(),
                // BufferLayout {
                //     step_func: VertexStep::PerInstance,
                //     ..Default::default()
                // },
            ],
            &[
                VertexAttribute::new("position", VertexFormat::Float3),
                VertexAttribute::new("color", VertexFormat::Float3),
            ],
            shader_3d,
            PipelineParams {
                cull_face: CullFace::Back,
                depth_test: Comparison::LessOrEqual,
                depth_write: true,

                // color_blend: Some(BlendState::new(
                //     Equation::Add,
                //     BlendFactor::Value(BlendValue::SourceAlpha),
                //     BlendFactor::OneMinusValue(BlendValue::SourceAlpha))
                // ),
                ..Default::default()
            },
        );

        // display
        let vertex_composite_buffer = ctx.new_buffer(
            BufferType::VertexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(&[
                    VertexComposite { position: [-1.0,  1.0] },
                    VertexComposite { position: [-1.0, -1.0] },
                    VertexComposite { position: [ 1.0, -1.0] },
                    VertexComposite { position: [ 1.0,  1.0] },
                ])
        );

        let index_composite_buffer = ctx.new_buffer(
            BufferType::IndexBuffer,
            BufferUsage::Immutable,
            BufferSource::slice(&[0u16, 1, 2, 0, 2, 3])
        );

        let display_bindings = Bindings {
            vertex_buffers: vec![vertex_composite_buffer],
            index_buffer: index_composite_buffer,
            images: vec![ui_rendering_texture, rendering_3d_texture],
        };

        let shader_composite = ctx
            .new_shader(
                ShaderSource::Glsl {
                    vertex: shaderComposite::VERTEX,
                    fragment: shaderComposite::FRAGMENT,
                },
                shaderComposite::meta(),
            )
            .unwrap();

        let display_pipeline = ctx.new_pipeline(
            &[
                BufferLayout::default(),
                // BufferLayout {
                //     step_func: VertexStep::PerInstance,
                //     ..Default::default()
                // },
            ],
            &[
                VertexAttribute::new("uv", VertexFormat::Float2),
            ],
            shader_composite,
            PipelineParams {
                // cull_face: CullFace::Back,
                // depth_test: Comparison::LessOrEqual,
                // depth_write: true,

                color_blend: Some(BlendState::new(
                    Equation::Add,
                    BlendFactor::Value(BlendValue::SourceAlpha),
                    BlendFactor::OneMinusValue(BlendValue::SourceAlpha))
                ),
                ..Default::default()
            },
        );

        MyMiniquadApp {
            display_pipeline,
            display_bindings,

            deffered_3d_pipeline,
            deffered_3d_bindings,
            deffered_3d_pass,

            deffered_ui_pipeline,
            deffered_ui_bindings,
            deffered_ui_pass,

            egui_mq: egui_miniquad::EguiMq::new(&mut *ctx),
            ctx,
            camera,
            input,
            indices_3d_count: ic3d,
            indices_ui_count: icui,
            tx_id
        }
    }
}

mod shader3D {
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

    pub const VERTEX: &str = include_str!("shaders/vertex_3d.glsl");

    // r#"#version 320
    // attribute vec3 in_pos;
    // attribute float typ;
    // attribute float rat;
    // attribute float char_index;
    // attribute float strip_offset;

    // varying lowp vec4 color;
    // varying mediump float typp;
    // varying mediump float ratt;
    // varying lowp vec2 point_cord;

    // uniform mediump mat4 mpv;
    // uniform mediump vec2 unit_size;

    // void main()
    // {
    //     typp = typ;
    //     gl_Position = mpv * vec4(in_pos * vec3(1.0, 1.0, -1.0), 1.0);
    //     if (typ == 1.0) {
    //         color = mix(
    //             vec4(0.1, 0.0, 0.05, 1.0),
    //             mix(vec4(0.3, 0.6, 0.5, 1.0), vec4(0.2, 0.4, 0.6, 1.0), rat),
    //             1.0 / clamp(gl_Position.z, 10.0, 10000.0) * 10.0
    //             // typ
    //         );
    //     } else if (typ == 2.0) {
    //         gl_Position.xyz /= gl_Position.w;
    //         gl_Position.z = clamp(gl_Position.z, -5.0, -1.0);
    //         // gl_Position.xyz /= gl_Position.w;
    //         gl_Position.w = 1.0;
    //         if (rat == 0.0) {
    //             gl_Position.xy += vec2(-0.5, 1.0) * unit_size * 20.0;
    //             point_cord = vec2(-1.0, 2.0);
    //         } else if (rat == 1.0) {
    //             gl_Position.xy += vec2(-0.5, -0.5) * unit_size * 20.0;
    //             point_cord = vec2(-1.0, -1.0);
    //         } else {
    //             gl_Position.xy += vec2(1.0, -0.5) * unit_size * 20.0;
    //             point_cord = vec2(2.0, -1.0);
    //         }
    //     } else if (typ == 3.0) {
    //         gl_Position.xyz /= gl_Position.w;
    //         gl_Position.z = clamp(gl_Position.z, -5.0, -1.0);
    //         // gl_Position.xyz /= gl_Position.w;
    //         gl_Position.w = 1.0;
    //         if (rat == 0.0) {
    //             gl_Position.xy += vec2(3.5 + (3.5 * char_index), 4.5) * unit_size * 3.5;
    //             point_cord = vec2(0.0 + strip_offset, 0.0);
    //         } else if (rat == 1.0) {
    //             gl_Position.xy += vec2(3.5 + (3.5 * char_index), -4.5) * unit_size * 3.5;
    //             point_cord = vec2(0.0 + strip_offset, 1.0);
    //         } else if (rat == 2.0) {
    //             gl_Position.xy += vec2(7.0 + (3.5 * char_index), -4.5) * unit_size * 3.5;
    //             point_cord = vec2((1.0 / 128.0) + strip_offset, 1.0);
    //         } else {
    //             gl_Position.xy += vec2(7.0 + (3.5 * char_index), 4.5) * unit_size * 3.5;
    //             point_cord = vec2((1.0 / 128.0) + strip_offset, 0.0);
    //         }
    //     }
    // }"#;

    pub const FRAGMENT: &str = include_str!("shaders/fragment_3d.glsl");
    // r#"#version 320
    // varying lowp vec4 color;
    // varying mediump float typp;
    // varying mediump float ratt;
    // varying lowp vec2 point_cord;

    // lowp float a;

    // uniform sampler2D tex;

    // void main() {
    //     gl_FragColor = color;
    //     if (typp == 2.0) {
    //         a = 1.0 - length(point_cord);
    //         gl_FragColor = mix(vec4(0.0), vec4(1.0), a * 2.0);
    //         // gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    //     } else if (typp == 3.0) {
    //         gl_FragColor = texture2D(tex, point_cord);
    //     }
    // }"#;

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

mod shaderUi {
    use miniquad::*;

    pub const VERTEX: &str = include_str!("shaders/vertex_ui.glsl");
    pub const FRAGMENT: &str = include_str!("shaders/fragment_ui.glsl");

    pub fn meta() -> ShaderMeta {
        ShaderMeta {
            images: vec!["ascii_strip".to_string()],
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

mod shaderComposite {
    use miniquad::*;

    pub const VERTEX: &str = include_str!("shaders/vertex_composite.glsl");
    pub const FRAGMENT: &str = include_str!("shaders/fragment_composite.glsl");

    pub fn meta() -> ShaderMeta {
        ShaderMeta {
            images: vec!["tex_ui".to_string(), "tex_3d".to_string()],
            uniforms: UniformBlockLayout {
                uniforms: vec![],
            },
        }
    }

    #[repr(C)]
    pub struct Uniforms {

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

        let right_movement: Vec3 = vec3(
            -rot.x.cos(),
            0.0,
            rot.x.sin()
        );

        //quick cam movement
        let now = Instant::now();
        let deltatime: f32 = now.duration_since(self.camera.last_frame_t).as_secs_f32();
        self.camera.last_frame_t = now;
        self.camera.position = self.camera.position + 20. * deltatime * ( forward * self.input.motion.y + right_movement * self.input.motion.x );

        let view = Mat4::look_at_rh(self.camera.position, self.camera.position + forward, up);
        let vs_3d_params = shader3D::Uniforms {
            mpv: self.camera.projection * view,
                // * (Mat4::from_rotation_y(self.camera.orientation.x)
                //     * Mat4::from_rotation_x(self.camera.orientation.y)),
        };

        let vs_ui_params = shaderUi::Uniforms {
            mpv: self.camera.projection * view,
                // * (Mat4::from_rotation_y(self.camera.orientation.x)
                //     * Mat4::from_rotation_x(self.camera.orientation.y)),
            unit_size: self.camera.unit_size
        };

        let vs_composite_params = shaderComposite::Uniforms {};

        //deffered
        self.ctx.begin_pass(
            Some(self.deffered_3d_pass),
            PassAction::clear_color(0.0, 0.0, 0.0, 0.0),
        );
        self.ctx.apply_pipeline(&self.deffered_3d_pipeline);
        self.ctx.apply_bindings(&self.deffered_3d_bindings);
        self.ctx.apply_uniforms(UniformsSource::table(&vs_3d_params));
        self.ctx.draw(0, self.indices_3d_count as i32, 1);
        self.ctx.end_render_pass();

        self.ctx.begin_pass(
            Some(self.deffered_ui_pass),
            PassAction::clear_color(0.0, 0.0, 0.0, 0.0),
        );
        self.ctx.apply_pipeline(&self.deffered_ui_pipeline);
        self.ctx.apply_bindings(&self.deffered_ui_bindings);
        self.ctx.apply_uniforms(UniformsSource::table(&vs_ui_params));
        self.ctx.draw(0, self.indices_ui_count as i32, 1);
        self.ctx.end_render_pass();


        //display
        self.ctx
            .begin_default_pass(mq::PassAction::clear_color(0.1, 0.0, 0.05, 1.0));
        self.ctx.apply_pipeline(&self.display_pipeline);
        self.ctx.apply_bindings(&self.display_bindings);
        // self.ctx.apply_uniforms(UniformsSource::table(&vs_composite_params));
        self.ctx.draw(0, 6, 1);
        self.ctx.end_render_pass();

        self.egui_mq.run(&mut *self.ctx, |_mq_ctx, egui_ctx| {
            // egui::Window::new("Egui Window").show(egui_ctx, |ui| {
            //     ui.heading("Hello World!");
            // });
            egui::SidePanel::right("my_right_panel").show(egui_ctx, |ui| {
                ui.with_layout(egui::Layout::top_down_justified(egui::Align::Center), |ui| {
                    ui.heading("Reaktory");
                    if ui.button("załaduj plik ze skoroszytem").clicked() {
                        let file = FileDialog::new()
                        .add_filter("excel", &["xlsx", "xls"])
                        .set_directory("/")
                        .pick_file();

                        if file.is_some() {
                            let file = file.unwrap();
                            let filepath = file.as_path().to_str().unwrap();
                            let mut workbook: Xlsx<_> = open_workbook(filepath).expect("Nie można otworzyć pliku.");
                            if let Ok(r) = workbook.worksheet_range("Arkusz1") {
                                for row in r.rows() {
                                    println!("row={:?}, row[0]={:?}", row, row[0]);
                                }
                            }
                        }
                    }
                })
            });
        });

        // Draw things behind egui here

        self.egui_mq.draw(&mut *self.ctx);

        // Draw things in front of egui here

        self.ctx.commit_frame();
    }

    fn resize_event(&mut self, width: f32, height: f32) {
        self.camera.projection = Mat4::perspective_rh_gl(120.0f32.to_radians(), width / height, 0.001, 100000.0);
        self.camera.screen_size = vec2(width, height);
    }

    fn mouse_motion_event(&mut self, x: f32, y: f32) {
        self.egui_mq.mouse_motion_event(x, y);
        let vek = vec2(x, y);
        if self.input.holding_rmb {
            self.camera.orientation += ((vek - self.input.last_mouse_position) / -180.) * PI;
            self.camera.orientation = vec2(self.camera.orientation.x % (2. * PI), self.camera.orientation.y.clamp((-0.5 * PI) + 0.0001, (0.5 * PI) + 0.0001));
        }
        self.input.last_mouse_position = vek;
    }

    fn mouse_wheel_event(&mut self, dx: f32, dy: f32) {
        self.egui_mq.mouse_wheel_event(dx, dy);
    }

    fn mouse_button_down_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_down_event(mb, x, y);
        if mb == mq::MouseButton::Right {
            self.input.holding_rmb = true;
        }
    }

    fn mouse_button_up_event(&mut self, mb: mq::MouseButton, x: f32, y: f32) {
        self.egui_mq.mouse_button_up_event(mb, x, y);
        if mb == mq::MouseButton::Right {
            self.input.holding_rmb = false;
        }
    }

    fn char_event(&mut self, character: char, _keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.char_event(character);
    }

    fn key_down_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods, _repeat: bool) {
        self.egui_mq.key_down_event(keycode, keymods);
        match keycode {
            mq::KeyCode::W => {
                self.input.motion.y += 1.;
            }
            mq::KeyCode::S => {
                self.input.motion.y -= 1.;
            }
            mq::KeyCode::A => {
                self.input.motion.x -= 1.;
            }
            mq::KeyCode::D => {
                self.input.motion.x += 1.;
            }
            _ => return,
        }
        self.input.motion.x = self.input.motion.x.clamp(-1., 1.);
        self.input.motion.y = self.input.motion.y.clamp(-1., 1.);
    }

    fn key_up_event(&mut self, keycode: mq::KeyCode, keymods: mq::KeyMods) {
        self.egui_mq.key_up_event(keycode, keymods);
        match keycode {
            mq::KeyCode::W => {
                self.input.motion.y -= 1.;
            }
            mq::KeyCode::S => {
                self.input.motion.y += 1.;
            }
            mq::KeyCode::A => {
                self.input.motion.x += 1.;
            }
            mq::KeyCode::D => {
                self.input.motion.x -= 1.;
            }
            _ => return,
        }
        self.input.motion.x = self.input.motion.x.clamp(-1., 1.);
        self.input.motion.y = self.input.motion.y.clamp(-1., 1.);
    }
}

fn main() {
    miniquad::start(mq::conf::Conf::default(), || Box::new(MyMiniquadApp::new()));
}
