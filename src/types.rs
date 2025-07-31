use glium::implement_vertex;
// use glium:: vertex::VertexBuffer;
use glam::{Vec2, Vec3, Mat4};
use std::time::Instant;

#[derive(Copy, Clone)]
pub struct Vertex3D {
    pub position: [f32; 3],
    pub color_ratio: f32
}

implement_vertex!(Vertex3D, position, color_ratio);

#[derive(Copy, Clone)]
pub struct VertexUi {
    pub position: [f32; 3],
    pub element_type: u8,
    pub vertex_type: u8,

    pub char_offset: i32,
    pub char_index: u8
}

implement_vertex!(VertexUi, position, element_type, vertex_type, char_offset, char_index);

pub struct Camera {
    pub position: Vec3,
    pub orientation: Vec2,
    pub projection: Mat4,

    pub last_frame_t: Instant,
    pub screen_size: (u32, u32),
    pub unit_size: Vec2
}

pub struct InputData {
    pub motion: Vec2,
    pub holding_rmb: bool
}
