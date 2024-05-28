#ifndef ENUMS_H
#define ENUMS_H

enum ActiveTechnique {
  NOTECH = -1,
  COLGLYPH = 0,
  GLYPHCOL = 1,
};


enum ColourMode {
  COMPLEMENTARY = 0,
  CONTRASTING = 1,
  MONOCHROMATIC = 2,
};


enum SaturationMode {
  SATURATED = 0,
  DESATURATED = 3,
};


enum GlyphStyle {
  CIRCLE = 0,
  TRIANGLE = 1,
  SQUARE = 2,
  CROSS = 3,
};

enum SamplingMode {
  FULLYSAMPLED = 0,
  REGULARLYSUBSAMPLED = 1,
  IRREGULARLYSUBSAMPLED = 2,
};

#endif
