use glam::{Vec2, Vec3, Mat4};
use std::time::Instant;

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