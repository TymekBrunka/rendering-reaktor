macro_rules! gen_cube {
    ($vertex_vec:tt, $indiecies_vec:tt, $indieciec_count:tt, $verticies_count:tt, $x:expr, $y:expr, $z:expr, $width:expr, $height:expr, $length:expr) => {
        $vertex_vec.reserve(36);
        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z ], rat: 1. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z ], rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z ], rat: 1. });

        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z + $length ], rat: 1. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z + $length ], rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z + $length ], rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z + $length ], rat: 1. });

        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count + 3);
        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 2);

        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 5);
        $indiecies_vec.push($verticies_count + 7);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 4);

        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 5);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 0);

        $indiecies_vec.push($verticies_count + 7);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 7);
        $indiecies_vec.push($verticies_count + 3);
        $indiecies_vec.push($verticies_count + 2);

        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 3);
        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 3);
        $indiecies_vec.push($verticies_count + 7);

        $indiecies_vec.push($verticies_count + 5);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count + 1);

        $verticies_count += 8;
        $indieciec_count += 36;
    };
}

pub(crate) use gen_cube;
