# 42_miniRT

> Ray tracer escrito do zero em C puro como projeto da [42 School](https://42.fr). Renderiza cenas 3D descritas em arquivos `.rt` com iluminação realista baseada no modelo de Phong.

**Stack:** `C` · `MLX42` · `pthreads` · `CMake` · `Make`  
**Escola:** 42 São Paulo  
**Status:** ✅ Concluído — obrigatório + todos os itens bonus implementados

---

<!-- Adicione aqui 2 ou 3 screenshots/gifs dos renders -->
> 📸 _Screenshots dos renders em breve_

---

## O que é Ray Tracing?

Ray tracing simula o comportamento físico da luz. Para cada pixel da tela, um raio é lançado da câmera em direção à cena. Quando atinge um objeto, a cor é calculada com base em iluminação, sombras e propriedades do material.

```
P(t) = origin + t * direction
```

O objeto visível em cada pixel é o de menor `t` positivo ao longo do raio.

---

## Compilação

```bash
make          # versão obrigatória
make bonus    # versão bonus
make debug    # debug
```

**Dependências:** `cmake`, `make`, `cc`, `libglfw3-dev`

O MLX42 é compilado automaticamente via CMake na primeira vez.

---

## Uso

```bash
./miniRT scenes/complex.rt
./miniRT_bonus scenes/bonus_full.rt
```

- `ESC` ou fechar a janela encerra o programa
- A janela suporta redimensionamento

---

## Funcionalidades

| Recurso | Obrigatório | Bonus |
|---|:---:|:---:|
| Esfera, Plano, Cilindro | ✅ | ✅ |
| Cone | — | ✅ |
| Iluminação de Phong (ambiente, difusa, especular) | ✅ | ✅ |
| Sombras (shadow rays) | ✅ | ✅ |
| Múltiplas luzes com acumulação | ✅ | ✅ |
| Anti-aliasing MSAA | — | ✅ |
| Reflexão recursiva | — | ✅ |
| Propriedades de material por objeto (ks, kd, ka, shininess, reflectivity) | — | ✅ |
| Checkerboard procedural | — | ✅ |
| Bump mapping (procedural via seno + PNG) | — | ✅ |
| Multi-threading com pthreads | — | ✅ |
| Cor do céu (miss ray) | — | ✅ |
| Janela redimensionável | ✅ | ✅ |

---

## Arquitetura

```
42_miniRT/
├── include/          Headers versão obrigatória
├── include_bonus/    Headers versão bonus
├── src/              Código fonte obrigatório (28 arquivos)
├── src_bonus/        Código fonte bonus (43 arquivos)
├── scenes/           Arquivos .rt de exemplo e teste
├── libft/            Biblioteca C customizada
└── MLX42/            Biblioteca gráfica (submodule)
```

### Módulos principais (`src/`)

- **`parser/`** — Lê e valida o arquivo `.rt` com tabela de lookup (`g_element_parsers[]`). Garante exatamente 1 câmera e 1 luz ambiente, com tratamento de erros em cada etapa.
- **`intersect/`** — Interseção raio-objeto para esfera (equação quadrática), plano (produto escalar) e cilindro (corpo + tampas).
- **`lighting/`** — Modelo de Phong: componentes ambiente, difusa e especular. Shadow rays para cada luz.
- **`vectors/`** — Biblioteca de vetores 3D: soma, subtração, escalar, produto vetorial, normalização.
- **`draw/`** — Loop de renderização pixel a pixel.
- **`color/`** — Operações com `t_color` (RGBA): adição, escala, clamp, conversão para `uint32_t`.

### Extras do bonus (`src_bonus/`)

- **`draw/draw_pthreads.c`** — renderização paralela com pthreads
- **`multithread/`** — workers de thread, gerenciamento da fila de tiles e debug
- **`draw/draw_utils.c`** — supersampling MSAA com média de cor
- **`intersect/hit_cone.c`** — interseção raio-cone com equação quadrática e verificação de altura
- **`intersect/checkerboard.c`** — padrão de tabuleiro de xadrez procedural em todos os objetos
- **`intersect/bump_mapping.c`** — perturbação da normal via textura procedural (seno) ou PNG
- **`intersect/bump_shapes.c`** — cálculo de tangente/bitangente por tipo de objeto
- **`parser/parser_cone.c`** — parse de objetos `cn` no arquivo `.rt`
- **`scene/object_extra_args.c`** — parâmetros extras de material: `ks`, `kd`, `ka`, `shininess`, `reflectivity`
- **`scene/object_defaults.c`** — aplica valores padrão de material quando omitidos
- **`scene/object_bump.c`** — carrega textura de bump map (PNG) via MLX42
- **`lighting/lighting_reflect.c`** — reflexão recursiva

---

## Formato das Cenas (`.rt`)

### Elementos globais (exatamente 1 de cada)

```
A <intensidade> <R,G,B>          # Luz ambiente
C <x,y,z> <dx,dy,dz> <fov>      # Câmera
L <x,y,z> <brilho> <R,G,B>      # Luz pontual (pode ter múltiplas)
```

### Objetos

```
sp <x,y,z> <diâmetro> <R,G,B>
pl <x,y,z> <nx,ny,nz> <R,G,B>
cy <x,y,z> <ax,ay,az> <diâmetro> <altura> <R,G,B>
cn <x,y,z> <ax,ay,az> <diâmetro> <altura> <R,G,B>   # bonus
```

### Parâmetros de material (bonus, opcionais)

```bash
# Apenas Phong
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity>

# Phong + checkerboard
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity> <R2,G2,B2> <checker_scale>

# Phong + checkerboard + bump mapping
sp ... <R,G,B> <ks> <kd> <ka> <shininess> <reflectivity> <R2,G2,B2> <checker_scale> <bump_scale> <bump_path>
# Use "none" como bump_path para bump procedural (seno)
```

Defaults quando omitidos: `KA=0.2` · `KD=0.7` · `KS=0.2` · `SHININESS=30` · `REFLECTIVITY=0`

### Exemplo de cena

```
A 0.3 255,255,255
C 0,5,-20 0,-0.2,1 60
L -20,30,-10 0.8 255,255,255

sp 0,0,15 6.0 255,0,0
pl 0,-3,0 0,1,0 200,200,200
cy 0,-3,2 0,1,0 2.0 6.0 255,128,0
```

### Validações

- Exatamente 1 câmera (`C`) e 1 luz ambiente (`A`) por arquivo
- Extensão `.rt` obrigatória
- Intensidades e brilho: `[0.0, 1.0]`
- Cores RGB: `[0, 255]`
- Vetores de orientação: componentes em `[-1.0, 1.0]`
- FOV: `[0, 180]`
- Diâmetro e altura: valores positivos

---

## Autores

| Colaborador | Contribuições principais |
|---|---|
| **rgregori** ([@rodrigo-americo](https://github.com/rodrigo-americo)) | Parser completo, lighting, cálculos geométricos, itens bonus (cone, MSAA, reflexão, bump mapping, checkerboard, materiais) |
| **tlavared** ([@Talen400](https://github.com/Talen400)) | Cálculos geométricos,  multi-threading com pthreads, estrutura geral |
