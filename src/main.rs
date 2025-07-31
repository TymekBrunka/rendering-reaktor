#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

mod glium_util;
mod types;

use egui::ViewportId;
use glium::{vertex::VertexBuffer, IndexBuffer};
use glium::glutin::{
    self,
    surface::WindowSurface,
    display::GetGlDisplay,
};
use winit::{
    application::ApplicationHandler,
    event::{StartCause, WindowEvent, KeyEvent, ElementState},
    event_loop::{ActiveEventLoop, EventLoop},
    window::{Window, WindowId},
    keyboard::{Key, NamedKey},
};

use glium_util::*;
use types::*;

struct Application {
    verticies_3d: VertexBuffer<Vertex3D>,
    index_3d: IndexBuffer<u16>,
    verticies_ui: VertexBuffer<VertexUi>,
    index_ui: IndexBuffer<u16>,
    camera: Camera,
    input: InputData
}

impl ApplicationContext for Application {
    const WINDOW_TITLE:&'static str = "Wizulizator stanu reaktorów biologicznych w 3D";
    fn new(display: &Display<WindowSurface>) -> Self {}
    fn draw_frame(&mut self, _display: &Display<WindowSurface>) { }
    fn resize_window(&mut self, _new_size: (u32, u32)) { }
    fn update(&mut self) { }
    fn handle_window_event(&mut self, _event: &glium::winit::event::WindowEvent, _window: &glium::winit::window::Window) { }
}

fn main() {

}
