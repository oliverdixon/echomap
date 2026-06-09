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

fn rot(a: f32) -> mat2x2<f32> {
    let s = sin(a);
    let c = cos(a);

    return mat2x2<f32>(
        vec2<f32>( c, s),
        vec2<f32>(-s, c),
    );
}

fn palette(t: f32) -> vec3<f32> {
    return params.colour_a +
           params.colour_b *
           cos(6.28318530718 * (params.colour_c * t + params.colour_d));
}

fn hash21(p: vec2<f32>) -> f32 {
    return fract(sin(dot(p, vec2<f32>(127.1, 311.7))) * 43758.5453123);
}

fn noise(p: vec2<f32>) -> f32 {
    let i = floor(p);
    let f = fract(p);
    let u = f * f * (vec2<f32>(3.0) - 2.0 * f);

    let a = hash21(i + vec2<f32>(0.0, 0.0));
    let b = hash21(i + vec2<f32>(1.0, 0.0));
    let c = hash21(i + vec2<f32>(0.0, 1.0));
    let d = hash21(i + vec2<f32>(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

fn fbm(p_in: vec2<f32>) -> f32 {
    var p = p_in;
    var value = 0.0;
    var amplitude = 0.5;

    for (var i: i32 = 0; i < 5; i = i + 1) {
        value += amplitude * noise(p);
        p = rot(0.55) * p * 2.03;
        amplitude *= 0.5;
    }

    return value;
}

fn star_field(p: vec2<f32>, t: f32) -> vec3<f32> {
    let q = p * 18.0;
    let cell = floor(q);
    let local = fract(q) - vec2<f32>(0.5);

    let h = hash21(cell);

    if (h < 0.965) {
        return vec3<f32>(0.0);
    }

    let d = length(local);
    let twinkle = 0.55 + 0.45 * sin(t * 5.0 + h * 80.0);
    let glow = exp(-90.0 * d) * twinkle;

    return palette(h + t * 0.02) * glow;
}

@fragment
fn fs_main(in: VsOut) -> @location(0) vec4<f32> {
    var uv = in.uv * 2.0 - vec2<f32>(1.0);
    uv.x *= params.viewport.y;

    let t = params.viewport.x * params.controls.x;
    let intensity = params.controls.y;
    let warp = params.controls.z;
    let scale = max(params.controls.w, 0.1);

    var p = uv * scale;

    let sky = mix(
        vec3<f32>(0.005, 0.010, 0.030),
        vec3<f32>(0.020, 0.030, 0.090),
        smoothstep(-1.0, 1.0, uv.y)
    );

    var colour = sky;
    colour += star_field(p + vec2<f32>(0.0, t * 0.015), t) * 0.9;

    for (var i: i32 = 0; i < 5; i = i + 1) {
        let fi = f32(i);

        let x = p.x * (0.9 + fi * 0.12);
        let n = fbm(vec2<f32>(
            x * 1.5 + t * (0.13 + fi * 0.04),
            fi * 7.3 + t * 0.05
        ));

        let wave = sin(x * (2.5 + fi * 0.35) + t * (0.8 + fi * 0.12));
        let curtain = -0.35 + fi * 0.17 + wave * 0.10 + (n - 0.5) * 0.45 * warp;

        let d = abs(p.y - curtain);
        let ribbon = exp(-18.0 * d) * smoothstep(-1.2, 0.9, p.y);
        let vertical_fade = smoothstep(-0.95, 0.55, p.y) * smoothstep(1.15, -0.1, p.y);

        colour += palette(fi * 0.13 + n * 0.2 + t * 0.04) *
                  ribbon *
                  vertical_fade *
                  (0.22 + fi * 0.05);
    }

    let vignette = smoothstep(1.55, 0.2, length(uv));
    colour *= vignette * intensity;

    colour = colour / (colour + vec3<f32>(1.0));
    colour = pow(colour, vec3<f32>(0.454545));

    return vec4<f32>(colour, 1.0);
}
