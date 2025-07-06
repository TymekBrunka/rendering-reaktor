macro_rules! gen_cube {
    ($vertex_vec:tt, $indiecies_vec:tt, $indieciec_count:tt, $verticies_count:tt, $x:expr, $y:expr, $z:expr, $width:expr, $height:expr, $length:expr) => {
        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z ], color: [0., 0., 1., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z ], color: [1., 0., 0., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z ], color: [0., 1., 0., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z ], color: [0., 0., 1., 1.] });

        $vertex_vec.push( Vertex { pos : [ $x, $y + $height, $z + $length ], color: [-2., 0., 1., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x, $y, $z + $length ], color: [-1., 0., 0., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y, $z + $length ], color: [-2., 1., 0., 1.] });
        $vertex_vec.push( Vertex { pos : [ $x + $width, $y + $height, $z + $length ], color: [-2., 0., 1., 1.] });

        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 1);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count + 2);
        $indiecies_vec.push($verticies_count);
        $indiecies_vec.push($verticies_count + 3);

        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 5);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 6);
        $indiecies_vec.push($verticies_count + 4);
        $indiecies_vec.push($verticies_count + 7);

        $verticies_count += 8;
        $indieciec_count += 12;
    };
}

pub(crate) use gen_cube;
