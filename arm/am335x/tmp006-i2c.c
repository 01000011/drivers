#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include "tmp006.h"

#define TMP006_I2C_ADDRESS	0x40

static const unsigned short tmp006_address_list[] = 
{ 
	TMP006_I2C_ADDRESS,
	I2C_CLIENT_END 
};

static int tmp006_i2c_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	if (client->addr != TMP006_I2C_ADDRESS)
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

static const struct i2c_device_id tmp006_i2c_device_id[] = {
	{ TMP006_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tmp006_i2c_device_id);

static struct i2c_driver tmp006_i2c_driver = {
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

module_i2c_driver(tmp006_i2c_driver);

MODULE_AUTHOR("Courtney Harleston <courtney.harleston@gmail.com>");
MODULE_DESCRIPTION("TMP006 I2C bus driver");
MODULE_LICENSE("GPL");
