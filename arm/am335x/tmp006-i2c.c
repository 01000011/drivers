#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include "tmp006.h"

#define ADR1_0_ADR0_0  		0x40
#define ADR1_0_ADR0_1		0x41
#define ADR1_0_ADR0_SDA		0x42
#define ADR1_0_ADR0_SCL		0x43
#define ADR1_1_ADR0_0		0x44
#define	ADR1_1_ADR0_1		0x45
#define	ADR1_1_ADR0_SDA		0x46
#define ADR1_1_ADR0_SCL		0x47

static const unsigned short tmp006_address_list[] = 
{ 
	ADR1_0_ADR0_0,
	I2C_CLIENT_END 
};

static int tmp006_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	if (client->addr != ADR1_0_ADR0_0)
	{
		return -ENODEV;
	}
	return tmp006_detect(&client->dev);
}

static int tmp006_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int err;
	struct regmap *regmap = devm_regmap_init_i2c(client, &tmp006_regmap_config);

	if (IS_ERR(regmap)) {
		err = PTR_ERR(regmap);
		dev_err(&client->dev, "Failed to init regmap: %d\n", err);
		return err;
	}

	return tmp006_probe(&client->dev, regmap);
}

static int tmp006_i2c_remove(struct i2c_client *client)
{
	return tmp006_remove(&client->dev);
}

static const struct i2c_device_id tmp006_i2c_device_id[] = 
{
	{ TMP006_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp006_i2c_device_id);

static struct i2c_driver tmp006_i2c_driver = 
{
	.driver = {
		.owner	= THIS_MODULE,
		.name	= TMP006_NAME,
	},
	.id_table	= tmp006_i2c_device_id,
	.probe		= tmp006_i2c_probe,
	.remove		= tmp006_i2c_remove,

	.detect		= tmp006_i2c_detect,
	.address_list	= tmp006_address_list
};

#ifdef _I2C_DRIVER
module_i2c_driver(tmp006_i2c_driver);
#else
static int __init init_tmp006(void)
{
	return i2c_add_driver(&tmp006_i2c_driver);
}

static void __exit exit_tmp006(void)
{
	i2c_del_driver(&tmp006_i2c_driver);
}

module_init(init_tmp006);
module_exit(exit_tmp006);
#endif
MODULE_AUTHOR("Courtney Harleston <courtney.harleston@gmail.com>");
MODULE_DESCRIPTION("TMP006 I2C bus driver");
MODULE_LICENSE("GPL");
