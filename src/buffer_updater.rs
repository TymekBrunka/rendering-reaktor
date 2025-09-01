use crate::types::*;

use crate::util::{gen_point, gen_right_side_text};
use miniquad::{
	GlContext,
	BufferId,
	BufferSource,
	RenderingBackend
};

pub fn gen_arrays(texts: &[String]) -> (Vec<VertexUi>, Vec<u16>, u16, u16) {
    let mut _icui: u16 = 0;
    let mut vcui: u16 = 0;
    let mut vui: Vec<VertexUi> = Vec::new();
    let mut iui: Vec<u16> = Vec::new();

    let mut j: usize = 0;


	for i in FloatIter(-17.5, 12.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -29.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -29., 12, texts[j][..].as_bytes());
        j += 1;
    }

    gen_point!(vui, iui, _icui, vcui, 17.5, 0.0, -24.5);
    gen_right_side_text!(vui, iui, _icui, vcui, 17.5, 0.0, -24.5, 12, texts[j][..].as_bytes());
    j += 1;

    gen_point!(vui, iui, _icui, vcui, 19., 0.0, -18.75);
    gen_right_side_text!(vui, iui, _icui, vcui, 19., 0.0, -18.75, 12, texts[j][..].as_bytes());
    j += 1;

    for i in FloatIter(-17.5, 12.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -13.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -13., 12, texts[j][..].as_bytes());
        j += 1;
    }

    for i in FloatIter(2.5, 17.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -4.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -4., 12, texts[j][..].as_bytes());
        j += 1;
    }
    (vui, iui, _icui, vcui)
}

pub fn update_buffer(ctx: &mut GlContext, row: &Row, buffer: BufferId) -> () {
	let a = row.KNKDs.clone().map(|x| {format!("{:012}", x)});
	let (vui, iui, icui, vcui) = gen_arrays(&a);
	ctx.buffer_update(
        buffer,
        BufferSource::slice(&vui),
    );
}