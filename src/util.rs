macro_rules! gen_cube {
    ($vertex_vec:tt, $indiecies_vec:tt, $indieciec_count:tt, $verticies_count:tt, $x:expr, $y:expr, $z:expr, $width:expr, $height:expr, $length:expr) => {
        $vertex_vec.reserve(8);
        $indiecies_vec.reserve(4);
        $vertex_vec.push( Vertex3D { position : [ $x, $y + $height, $z ], color_ratio: 1.});
        $vertex_vec.push( Vertex3D { position : [ $x, $y, $z ], color_ratio: 0.});
        $vertex_vec.push( Vertex3D { position : [ $x + $width, $y, $z ], color_ratio: 0.});
        $vertex_vec.push( Vertex3D { position : [ $x + $width, $y + $height, $z ], color_ratio: 1.});

        $vertex_vec.push( Vertex3D { position : [ $x, $y + $height, $z + $length ], color_ratio: 1.});
        $vertex_vec.push( Vertex3D { position : [ $x, $y, $z + $length ], color_ratio: 0.});
        $vertex_vec.push( Vertex3D { position : [ $x + $width, $y, $z + $length ], color_ratio: 0.});
        $vertex_vec.push( Vertex3D { position : [ $x + $width, $y + $height, $z + $length ], color_ratio: 1.});

        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count + 3);

        $verticies_count += 8;
        $indieciec_count += 4;
    };
}


pub(crate) use {gen_cube};
