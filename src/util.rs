macro_rules! gen_cube {
    ($vertex_vec:tt, $indiecies_vec:tt, $indieciec_count:tt, $verticies_count:tt, $x:expr, $y:expr, $z:expr, $width:expr, $height:expr, $length:expr) => {
        $vertex_vec.reserve(36);
        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z ], typ: 1.0, rat: 1. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], typ: 1.0, rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z ], typ: 1.0, rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z ], typ: 1.0, rat: 1. });

        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z + $length ], typ: 1.0, rat: 1. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z + $length ], typ: 1.0, rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z + $length ], typ: 1.0, rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z + $length ], typ: 1.0, rat: 1. });

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

macro_rules! gen_point {
    ($vertex_vec:tt, $indiecies_vec:tt, $indieciec_count:tt, $verticies_count:tt, $x:expr, $y:expr, $z:expr) => {
        $vertex_vec.reserve(3);
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], typ: 2.0, rat: 0. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], typ: 2.0, rat: 1. });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], typ: 2.0, rat: 2. });

        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 2);

        $verticies_count += 3;
        $indieciec_count += 3;
    }
}

pub(crate) use {gen_cube, gen_point};
