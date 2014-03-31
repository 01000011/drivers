#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include "tmp006.h"
#define VOBJECT_REG         0x00
#define TAMBIENT_REG        0x01
#define CONFIGURATION_REG   0x02
#define MANUFACTURER_ID_REG 0xFE
#define DEVICE_ID_REG   0xFF
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
	struct  tmp006_config_data config_data;
	u16 device_id;
};

static int init_device(struct tmp006 *dev)
{
	return 0;
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

	result = regmap_read(data->regmap, DEVICE_ID_REG, &id);
	if (result < 0)
	{
		return result;
	}
	if (id != data->device_id)
	{
		return -ENODEV;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(tmp006_detect);

int tmp006_probe(struct device *dev, struct regmap *regmap)
{
	struct tmp006 *data;
	int err;
	printk(KERN_INFO "driver_probe");	

	data = kzalloc(sizeof(struct tmp006), GFP_KERNEL);
	if (!data) 
	{
		err = -ENOMEM;
		goto exit;
	}

	dev_set_drvdata(dev, data);
	data->dev = dev;
	data->regmap = regmap;

	err = init_device(data);
	if (err < 0)
	{
		goto exit_free;
	}

	err = tmp006_detect(dev);
	if (err < 0) {
		dev_err(dev, "%s: chip_id failed!\n", TMP006_NAME);
		goto exit_free;
	}

	/* Register sysfs hooks */
	err = sysfs_create_group(&dev->kobj, &tmp006_attr_group);
	if (err)
		goto exit_free;

	dev_info(dev, "Successfully initialized %s!\n", TMP006_NAME);

	return 0;

exit_free:
	kfree(data);
exit:
	return err;
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
/*static int driver_remove(struct i2c_client *client)
{
	printk(KERN_INFO "driver_remove");
	return 0;
}
static int __init _driver_init(void)
{
	printk(KERN_INFO "driver_init");
	return i2c_add_driver(&tmp006_driver);
}

static void __exit _driver_exit(void)
{
	printk(KERN_INFO "driver_exit");
	i2c_del_driver(&tmp006_driver);
}*/
MODULE_LICENSE("GPL");

/*module_init(_driver_init);
module_exit(_driver_exit);*/
