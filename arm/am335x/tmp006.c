#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include "tmp006.h"

#define VOBJECT_REG         0x00
#define TAMBIENT_REG        0x01
#define CONFIGURATION_REG   0x02
#define MANUFACTURER_ID_REG 0xFE
#define DEVICE_ID_REG       0xFF

#define RESET          0x8000
#define POWER_DOWN     0x0000
#define POWER_UP       0x7000
#define TMP006_CR_4         0x0000
#define TMP006_CR_2         0x0200
#define TMP006_CR_1         0x0400
#define TMP006_CR_0_5       0x0600
#define TMP006_CR_0_25 0x0800
#define ENABLE         0x0100
#define DATA_READY     0x0080
static struct tmp006_config_data
{
	u16 v_object;
	u16 t_ambient;
	u16 configuration;
	u16 manufacturer_id;
	u16 device_id;
};

static struct tmp006
{
	struct	device *dev;
	struct  regmap *regmap;
	struct	mutex lock;
	//struct  tmp006_config_data config_data;
};

static int read (struct device *dev, unsigned int reg, unsigned int *val)
{
	struct tmp006 *t6 = dev_get_drvdata(dev);
	return regmap_read(t6->regmap, DEVICE_ID_REG, val);
}

static int write(struct device *dev, unsigned int reg, unsigned int val)
{
	struct tmp006 *t6 = dev_get_drvdata(dev);
	return regmap_write(t6->regmap, reg, val);
}

static int read_temperature(struct tmp006 *dev, int *temperature)
{
	return 0;
}

static ssize_t get_temperature(struct device *dev, struct device_attribute *attr, char *buf)
{
	int temperature;
	int result;	
	struct tmp006 *data;

	temperature = 0;
	data = dev_get_drvdata(dev);
	result = read_temperature(data, &temperature);
	if (result < 0)
	{
		return result;
	}
	else
	{
		return sprintf(buf, "%d\n", temperature);
	}
}

static DEVICE_ATTR(temp0_input, S_IRUGO, get_temperature, NULL);

static struct attribute *tmp006_attributes[] = 
{
	&dev_attr_temp0_input.attr,
	NULL
};

static const struct attribute_group tmp006_attr_group = 
{
	.attrs = tmp006_attributes,
};

int tmp006_detect(struct device *dev)
{
	struct tmp006 *data = dev_get_drvdata(dev);
	unsigned int id;
	int result;
	printk(KERN_INFO "tmp006_detect");
	/*result = regmap_read(data->regmap, DEVICE_ID_REG, &id);
	if (result < 0)
	{
		return result;
	}
	if (id != data->device_id)
	{
		return -ENODEV;
	}*/
	return 0;
}
EXPORT_SYMBOL_GPL(tmp006_detect);

int tmp006_probe(struct device *dev, struct regmap *regmap)
{
	int val;
	int status;
	struct tmp006 *data;
	printk(KERN_INFO "driver_probe");	

	data = devm_kzalloc(dev, sizeof(struct tmp006), GFP_KERNEL);
	if (data == NULL) 
	{
		dev_err(dev, "Can not allocate memory\n");
		status = -ENOMEM;
		goto exit;
	}

	dev_set_drvdata(dev, data);
	data->dev = dev;
	data->regmap = regmap;
	
	status = write(data->dev, CONFIGURATION_REG, POWER_UP);
	if(status < 0)
	{
		dev_err(data->dev, "could not reset device\n");
		goto exit_free;	
	}

	status = write(data->dev, CONFIGURATION_REG, POWER_UP | ENABLE);
	if(status < 0)
	{
		dev_err(data->dev, "could not initiate device\n");
		goto exit_free;	
	}
	
	status = read(data->dev, DEVICE_ID_REG, &val);
	if(status < 0)
	{
		dev_err(data->dev, "could not read device\n");
		goto exit_free;
	}
	
	dev_info(data->dev, "Successfully initialized!\n");
	return 0;
exit_free:
	//kfree(data);
exit:
	return status;
}

EXPORT_SYMBOL_GPL(tmp006_probe);

int tmp006_remove(struct device *dev)
{
	struct tmp006 *data = dev_get_drvdata(dev);

	sysfs_remove_group(&data->dev->kobj, &tmp006_attr_group);
	kfree(data);

	return 0;
}
EXPORT_SYMBOL_GPL(tmp006_remove);

struct regmap_config tmp006_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16
};
EXPORT_SYMBOL_GPL(tmp006_regmap_config);
MODULE_LICENSE("GPL");
