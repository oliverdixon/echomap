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

fn hash21(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453123);
}

fn hash22(p: vec2<f32>) -> vec2<f32> {
    return fract(sin(vec2<f32>(
        dot(p, vec2<f32>(127.1, 311.7)),
        dot(p, vec2<f32>(269.5, 183.3))
    )) * 43758.5453123);
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

    var p = uv;
    p = rot(0.08 * sin(t * 0.3)) * p;

    let ripple = vec2<f32>(
        sin(p.y * 4.0 + t * 0.7),
        cos(p.x * 4.0 - t * 0.5)
    ) * 0.18 * warp;

    p = (p + ripple) * (5.0 * scale);

    let cell = floor(p);
    let local = fract(p);

    var nearest = 10.0;
    var second_nearest = 10.0;
    var nearest_id = 0.0;

    for (var y: i32 = -1; y <= 1; y = y + 1) {
        for (var x: i32 = -1; x <= 1; x = x + 1) {
            let offset = vec2<f32>(f32(x), f32(y));
            let neighbour = cell + offset;

            let h = hash22(neighbour);
            let animated_point = 0.5 + 0.35 * sin(
                t * 0.8 +
                6.28318530718 * h +
                vec2<f32>(0.0, 1.7)
            );

            let delta = offset + animated_point - local;
            let d = length(delta);

            if (d < nearest) {
                second_nearest = nearest;
                nearest = d;
                nearest_id = hash21(neighbour);
            } else if (d < second_nearest) {
                second_nearest = d;
            }
        }
    }

    let border_distance = second_nearest - nearest;
    let borders = smoothstep(0.085, 0.0, border_distance);

    let nodes = exp(-24.0 * nearest);
    let cell_glow = exp(-5.0 * border_distance);

    var colour = vec3<f32>(0.005, 0.008, 0.020);

    colour += palette(nearest_id + t * 0.04) * borders * 0.85;
    colour += palette(nearest_id * 0.7 + t * 0.08) * nodes * 0.75;
    colour += palette(border_distance * 2.0 + t * 0.03) * cell_glow * 0.10;

    let scan = 0.94 + 0.06 * sin(in.uv.y * params.viewport.w * 0.75 + t * 60.0);
    colour *= scan;

    let radial = length(uv);
    let pulse = exp(-8.0 * abs(fract(radial * 2.5 - t * 0.35) - 0.5));
    colour += palette(radial + t * 0.05) * pulse * 0.08;

    let vignette = smoothstep(1.5, 0.2, length(uv));
    colour *= vignette * intensity;

    colour = colour / (colour + vec3<f32>(1.0));
    colour = pow(colour, vec3<f32>(0.454545));

    return vec4<f32>(colour, 1.0);
}
