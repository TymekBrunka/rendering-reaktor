use glam::{Vec2, Vec3, Mat4};
use std::time::Instant;
// use egui::Ui;

use egui_extras::TableRow;

use miniquad::{BufferId, RenderingBackend};
use crate::buffer_updater::update_buffer;

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

#[derive(Clone)]
pub struct Row {
    pub dataczas: String,
    pub KNKD: [f64; 20],
    pub KNKDs: [String; 20]
}

impl Default for Row {
    fn default() -> Self {
        Self {
            dataczas: Default::default(),
            KNKD: [0.0; 20],
            KNKDs: [
                String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"),
                String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"),
                String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"),
                String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"),
                String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"), String::from("0,00mg/l"),
            ]
        }
    }
}

impl Row {
    pub fn draw(&self, row: &mut TableRow<'_, '_>, ctx: &mut dyn RenderingBackend, buffer: BufferId) {
        row.col(|ui| {
            if ui.button(&self.dataczas).clicked() {
                update_buffer(ctx, self, buffer)
            }
        });
        for i in &self.KNKDs {
            row.col(|ui| {ui.label(i);});
        }
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