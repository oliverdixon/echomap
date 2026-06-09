struct Params {
    viewport: vec4<f32>,
    controls: vec4<f32>,
    colour_a: vec3<f32>,
    colour_b: vec3<f32>,
    colour_c: vec3<f32>,
    colour_d: vec3<f32>,
};

@group(0) @binding(0)
var<uniform> params: Params;

struct VsOut {
    @builtin(position) position: vec4<f32>,
    @location(0) uv: vec2<f32>,
};

@vertex
fn vs_main(@builtin(vertex_index) vertex_index: u32) -> VsOut {
    let positions = array<vec2<f32>, 3>(
        vec2<f32>(-1.0, -1.0),
        vec2<f32>( 3.0, -1.0),
        vec2<f32>(-1.0,  3.0),
    );

    let p = positions[vertex_index];

    var out: VsOut;
    out.position = vec4<f32>(p, 0.0, 1.0);
    out.uv = p * 0.5 + vec2<f32>(0.5);
    return out;
}

fn palette(t: f32) -> vec3<f32> {
    return params.colour_a +
           params.colour_b *
           cos(6.28318530718 * (params.colour_c * t + params.colour_d));
}

fn rot(a: f32) -> mat2x2<f32> {
    let s = sin(a);
    let c = cos(a);

    return mat2x2<f32>(
        vec2<f32>( c, s),
        vec2<f32>(-s, c),
    );
}

@fragment
fn fs_main(in: VsOut) -> @location(0) vec4<f32> {
    var uv = in.uv * 2.0 - vec2<f32>(1.0);
    uv.x *= params.viewport.y;

    let t = params.viewport.x * params.controls.x;
    let intensity = params.controls.y;
    let warp = params.controls.z;
    let scale = max(params.controls.w, 0.1);

    var p = uv * (1.35 / scale);
    p = rot(0.15 * sin(t * 0.21)) * p;

    let c = vec2<f32>(
        -0.78 + 0.18 * sin(t * 0.27),
         0.16 + 0.18 * cos(t * 0.19)
    );

    var z = p;
    var escaped = false;
    var iter = 0.0;
    var trap = 10.0;

    for (var i: i32 = 0; i < 128; i = i + 1) {
        let x = z.x * z.x - z.y * z.y;
        let y = 2.0 * z.x * z.y;

        z = vec2<f32>(x, y) + c;

        let r2 = dot(z, z);
        trap = min(trap, abs(length(z) - 0.65));

        if (r2 > 16.0) {
            escaped = true;
            iter = f32(i);
            break;
        }
    }

    let n = iter / 128.0;
    let edge = exp(-18.0 * trap);

    var colour = vec3<f32>(0.0);

    if (escaped) {
        colour += palette(n * 1.6 + t * 0.04) * pow(n, 0.35) * 0.9;
    } else {
        colour += vec3<f32>(0.0, 0.0, 0.01);
    }

    colour += palette(edge * 0.35 + t * 0.07) * edge * 0.75 * warp;

    let r = length(uv);
    let pulse = 0.5 + 0.5 * sin(18.0 * r - t * 4.0);
    colour += palette(pulse * 0.15 + t * 0.03) * exp(-2.0 * r) * 0.12;

    let vignette = smoothstep(1.45, 0.15, length(uv));
    colour *= vignette * intensity;

    colour = colour / (colour + vec3<f32>(1.0));
    colour = pow(colour, vec3<f32>(0.454545));

    return vec4<f32>(colour, 1.0);
}
