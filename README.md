# Metric Trainer

Interactive terminal program for users familiar with the imperial measurement system to learn the metric system (and vice versa, I suppose). I may come up with a better name for it someday...

## Features

- **Multiple Categories**: Distance, Weight, Temperature, Volume conversions
- **Bidirectional Practice**: Convert both metric ↔ imperial and imperial ↔ metric
- **Difficulty Modes**: 
  - Normal mode (default tolerance: 1%)
  - Whole numbers only (`--whole`)
  - Easy mode (`--easy`) with simple numbers and higher tolerance (5%)
- **Persistent Statistics**: Cross-session statistics saved to file, viewable with `stats` command

## Usage

```bash
./metric-trainer              # Start interactive mode
./metric-trainer --help       # Show help and options
./metric-trainer --whole      # Practice with whole numbers only
./metric-trainer --easy       # Practice with simple numbers and higher tolerance
```

### Interactive Commands

Once running, type:
- Category letters: `a` (distance), `b` (weight), `c` (temperature), `d` (volume)
- `all` - Practice all categories
- `help` - Show detailed help menu
- `stats` - View persistent statistics by category
- `quit` - Exit the program

Or you can combine letters to practice multiple categories (e.g. `ab` for distance and temperature).

## Building

```bash
make
```
