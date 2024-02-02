# Display simulator for Wingsuit

### Installation

```shell
pip3 install -r requirements.txt
```

### Usage

1) Copy-paste FlySight1 track into `data.csv` file
2) Modify `references.json` with coordinates of DZ and flight target
3) Set `hmsl` parameter - the ground elevation above MSL. It's required for calculating correct altitude AGL
4) In `elements.json` define which metrics will be displayed and where
5) launch the program

```shell
python3 main.py
```

### Supported readings

| Name         | Definition                          |
|--------------|-------------------------------------|
| `hmsl`       | Height above Mean Sea Level         |
| `alt`        | Height above Ground Level           |
| `gl`         | Glide ratio                         |
| `hspeed`     | Horizontal speed                    |
| `vspeed`     | Vertical speed                      |
| `totalSpeed` | Total speed ( _sqrt(Vh^2 + Vd^2)_ ) |

#### Readings configuration

| Name        | Definition                    | Required  |
|-------------|-------------------------------|-----------|
| `x`         | X coordinate                  | Yes       |
| `x`         | Y coordinate                  | Yes       |
| `font_size` | Text size in pt.              | Yes       |
| `font_id`   | `1` for *Bold* `2` for normal | Yes       |
| `color`     | Text color                    | No        |


Additionally you can put arbitrary text labels on the screen. Define them in `labels` block.
They use the same properties as Readings.
