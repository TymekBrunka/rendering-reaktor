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

    for i in FloatIter(2.5, 17.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -44.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -44., 12, texts[3 - j][..].as_bytes());
        j += 1;
    }

    // gen_point!(vui, iui, _icui, vcui, 19., 0.0, -58.75);
    // gen_right_side_text!(vui, iui, _icui, vcui, 19., 0.0, -58.75, 12, texts[j][..].as_bytes());
    // j += 1;

    for i in FloatIter(-17.5, 17.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -53.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -53., 12, texts[15 - j][..].as_bytes());
        j += 1;
    }

	for i in FloatIter(-17.5, 12.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, -69.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, -69., 12, texts[j][..].as_bytes());
        j += 1;
    }

    gen_point!(vui, iui, _icui, vcui, 17.5, 0.0, -64.5);
    gen_right_side_text!(vui, iui, _icui, vcui, 17.5, 0.0, -64.5, 12, texts[j][..].as_bytes());
    j += 1;

    //
    // reaktor 2 ----------------------------------------------------------------------------------------
    //

        for i in FloatIter(2.5, 17.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, 44.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, 44., 12, texts[43 - j][..].as_bytes());
        j += 1;
    }

    // gen_point!(vui, iui, _icui, vcui, 19., 0.0, -58.75);
    // gen_right_side_text!(vui, iui, _icui, vcui, 19., 0.0, -58.75, 12, texts[j][..].as_bytes());
    // j += 1;

    for i in FloatIter(-17.5, 17.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, 53.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, 53., 12, texts[55 - j][..].as_bytes());
        j += 1;
    }

    for i in FloatIter(-17.5, 12.5, 5.) {
        gen_point!(vui, iui, _icui, vcui, i, 0.0, 69.);
        gen_right_side_text!(vui, iui, _icui, vcui, i, 0.0, 69., 12, texts[j][..].as_bytes());
        j += 1;
    }

    gen_point!(vui, iui, _icui, vcui, 17.5, 0.0, 64.5);
    gen_right_side_text!(vui, iui, _icui, vcui, 17.5, 0.0, 64.5, 12, texts[j][..].as_bytes());
    j += 1;

    (vui, iui, _icui, vcui)
}

pub fn update_buffer(ctx: &mut dyn RenderingBackend, row: &Row, buffer: BufferId) -> () {
	let a = row.KNKDs.clone().map(|x| {format!("{:0>12}", x)});
	let (vui, _iui, _icui, _vcui) = gen_arrays(&a);
	ctx.buffer_update(
        buffer,
        BufferSource::slice(&vui),
    );
}