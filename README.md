> Ray tracer written from scratch in pure C as a [42 School](https://42.fr) project. Renders 3D scenes described in `.rt` files with realistic lighting based on the Phong model.

**Stack:** `C` · `MLX42` · `pthreads` · `CMake` · `Make`
**School:** 42 São Paulo
**Status:** ✅ Completed — mandatory + all bonus items implemented

---

<!-- Add 2 or 3 screenshots/gifs of the renders here -->
> 📸 _Screenshots of renders coming soon_

---

## What is Ray Tracing?

Ray tracing simulates the physical behavior of light. For each pixel on the screen, a ray is cast from the camera toward the scene. When it hits an object, the color is calculated based on lighting, shadows, and material properties.

```
P(t) = origin + t * direction
```

The visible object at each pixel is the one with the smallest positive `t` along the ray.

---

## Building

```bash
make          # mandatory version
make bonus    # bonus version
make debug    # debug
```

**Dependencies:** `cmake`, `make`, `cc`, `libglfw3-dev`

MLX42 is compiled automatically via CMake on the first run.

---

## Usage

```bash
./miniRT scenes/complex.rt
./miniRT_bonus scenes/bonus_full.rt
```

- `ESC` or closing the window exits the program
- The window supports resizing

---

## Features

| Feature | Mandatory | Bonus |
|---|:---:|:---:|
| Sphere, Plane, Cylinder | ✅ | ✅ |
| Cone | — | ✅ |
| Phong Lighting (ambient, diffuse, specular) | ✅ | ✅ |
| Shadows (shadow rays) | ✅ | ✅ |
| Multiple lights with accumulation | ✅ | ✅ |
| MSAA Anti-aliasing | — | ✅ |
| Recursive Reflection | — | ✅ |
| Per-object material properties (ks, kd, ka, shininess, reflectivity) | — | ✅ |
| Procedural Checkerboard | — | ✅ |
| Bump mapping (procedural via sine + PNG) | — | ✅ |
| Multi-threading with pthreads | — | ✅ |
| Sky color (miss ray) | — | ✅ |
| Resizable window | ✅ | ✅ |

---

## Architecture

```
42_miniRT/
├── include/          Mandatory version headers
├── include_bonus/    Bonus version headers
├── src/              Mandatory source code (28 files)
├── src_bonus/        Bonus source code (43 files)
├── scenes/           Sample and test .rt files
├── libft/            Custom C library
└── MLX42/            Graphics library (submodule)
```

### Main modules (`src/`)

- **`parser/`** — Reads and validates the `.rt` file using a lookup table (`g_element_parsers[]`). Ensures exactly 1 camera and 1 ambient light, with error handling at each step.
- **`intersect/`** — Ray-object intersection for sphere (quadratic equation), plane (dot product), and cylinder (body + caps).
- **`lighting/`** — Phong model: ambient, diffuse, and specular components. Shadow rays for each light.
- **`vectors/`** — 3D vector library: addition, subtraction, scalar, cross product, normalization.
- **`draw/`** — Pixel-by-pixel rendering loop.
- **`color/`** — Operations on `t_color` (RGBA): addition, scaling, clamping, conversion to `uint32_t`.

### Bonus extras (`src_bonus/`)

- **`draw/draw_pthreads.c`** — Parallel rendering with pthreads
- **`multithread/`** — Thread workers, tile queue management and debug
- **`draw/draw_utils.c`** — MSAA supersampling with color averaging
- **`intersect/hit_cone.c`** — Ray-cone intersection with quadratic equation and height check
- **`intersect/checkerboard.c`** — Procedural checkerboard pattern on all objects
- **`intersect/bump_mapping.c`** — Normal perturbation via procedural texture (sine) or PNG
- **`intersect/bump_shapes.c`** — Tangent/bitangent calculation per object type
- **`parser/parser_cone.c`** — Parsing of `cn` objects in the `.rt` file
- **`scene/object_extra_args.c`** — Extra material parameters: `ks`, `kd`, `ka`, `shininess`, `reflectivity`
- **`scene/object_defaults.c`** — Applies default material values when omitted
- **`scene/object_bump.c`** — Loads bump map texture (PNG) via MLX42
- **`lighting/lighting_reflect.c`** — Recursive reflection

---

## Scene Format (`.rt`)

### Global elements (exactly 1 of each)

```
A <intensity> <R,G,B>          # Ambient light
C <x,y,z> <dx,dy,dz> <fov>    # Camera
L <x,y,z> <brightness> <R,G,B> # Point light (multiple allowed)
```

### Objects

```
sp <x,y,z> <diameter> <R,G,B>
pl <x,y,z> <nx,ny,nz> <R,G,B>
cy <x,y,z> <ax,ay,az> <diameter> <height> <R,G,B>
cn <x,y,z> <ax,ay,az> <diameter> <height> <R,G,B>   # bonus
```

### Material parameters (bonus, optional)

```bash
# Phong only
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity>

# Phong + checkerboard
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity> <R2,G2,B2> <checker_scale>

# Phong + checkerboard + bump mapping
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity> <R2,G2,B2> <checker_scale> <bump_scale> <bump_path>
# Use "none" as bump_path for procedural bump (sine)
```

Defaults when omitted: `KA=0.2` · `KD=0.7` · `KS=0.2` · `SHININESS=30` · `REFLECTIVITY=0`

### Scene example

```
A 0.3 255,255,255
C 0,5,-20 0,-0.2,1 60
L -20,30,-10 0.8 255,255,255

sp 0,0,15 6.0 255,0,0
pl 0,-3,0 0,1,0 200,200,200
cy 0,-3,2 0,1,0 2.0 6.0 255,128,0
```

### Validations

- Exactly 1 camera (`C`) and 1 ambient light (`A`) per file
- `.rt` extension required
- Intensities and brightness: `[0.0, 1.0]`
- RGB colors: `[0, 255]`
- Orientation vectors: components in `[-1.0, 1.0]`
- FOV: `[0, 180]`
- Diameter and height: positive values

---

## Authors

| Contributor | Main contributions |
|---|---|
| **rgregori** ([@rodrigo-americo](https://github.com/rodrigo-americo)) | Full parser, lighting, geometric calculations, bonus items (cone, MSAA, reflection, bump mapping, checkerboard, materials) |
| **tlavared** ([@Talen400](https://github.com/Talen400)) | Geometric calculations, multi-threading with pthreads, overall structure |
