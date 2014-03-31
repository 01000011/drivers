#ifndef _TMP006_H
#define _TMP006_H

#include <linux/regmap.h>

#define TMP006_NAME "tmp006-driver"

extern struct regmap_config tmp006_regmap_config;

int tmp006_probe(struct device *dev, struct regmap *regmap);
int tmp006_remove(struct device *dev);
int tmp006_detect(struct device *dev);

#endif
