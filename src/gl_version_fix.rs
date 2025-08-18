use miniquad::*;
use miniquad::graphics;

pub fn new_gl_context() -> GlContext {
        unsafe {
            let mut default_framebuffer: GLuint = 0;
            glGetIntegerv(
                GL_FRAMEBUFFER_BINDING,
                &mut default_framebuffer as *mut _ as *mut _,
            );
            let mut vao = 0;

            glGenVertexArrays(1, &mut vao as *mut _);
            glBindVertexArray(vao);
            let info = gl_info();
            GlContext {
                default_framebuffer,
                shaders: ResourceManager::default(),
                pipelines: ResourceManager::default(),
                passes: ResourceManager::default(),
                buffers: ResourceManager::default(),
                textures: Textures(vec![]),
                info,
                cache: GlCache {
                    stored_index_buffer: 0,
                    stored_index_type: None,
                    stored_vertex_buffer: 0,
                    index_buffer: 0,
                    index_type: None,
                    vertex_buffer: 0,
                    cur_pipeline: None,
                    cur_pass: None,
                    color_blend: None,
                    alpha_blend: None,
                    stencil: None,
                    color_write: (true, true, true, true),
                    cull_face: CullFace::Nothing,
                    stored_texture: 0,
                    stored_target: 0,
                    textures: [CachedTexture {
                        target: 0,
                        texture: 0,
                    }; MAX_SHADERSTAGE_IMAGES],
                    attributes: [None; MAX_VERTEX_ATTRIBUTES],
                },
            }
        }
    }