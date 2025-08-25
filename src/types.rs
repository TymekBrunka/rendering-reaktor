use glam::{Vec2, Vec3, Mat4};
use std::time::Instant;
use egui::Ui;

// #[derive(Copy, Clone)]
pub struct Vertex3D {
    pub position: [f32; 3],
    pub color: [f32; 3],
}

// #[derive(Copy, Clone)]
pub struct VertexUi {
    pub position: [f32; 3],
    pub element_type: f32,
    pub vertex_type: f32,

    pub char_offset: f32,
    pub char_index: f32
}

pub struct VertexComposite {
    pub position: [f32; 2]
}

pub struct Camera {
    pub position: Vec3,
    pub orientation: Vec2,
    pub projection: Mat4,

    pub last_frame_t: Instant,
    pub screen_size: Vec2,
    pub unit_size: Vec2
}

pub struct InputData {
    pub motion: Vec2,
    pub holding_rmb: bool,
    pub last_mouse_position: Vec2
}

#[derive(Default)]
pub struct Row {
    pub dataczas: String,
    pub rb1ko_po4: f64,
    pub rb1ko_nh4: f64,
    pub rb2ko_po4: f64,
    pub rb2ko_nh4: f64,
}

impl Row {
    pub fn draw(self, ui: &mut Ui, index: i32) {
        if ui.button("clik me").clicked() {
            println!("{}", index);
        }
        ui.end_row();
    }
}

pub struct FloatIter(pub f32, pub f32, pub f32);  // start, end, and step

impl Iterator for FloatIter {
    type Item = f32;

    #[inline]
    fn next(&mut self) -> Option<f32> {
        if self.0 <= self.1 {
            let v = self.0;
            self.0 = v + self.2;
            Some(v)
        } else {
            None
        }
    }
}