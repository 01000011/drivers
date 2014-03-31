#include <linux/module.h>
#include <linux/i2c.h>
static int __init _driver_init(void);
static void __exit _driver_exit(void);
static int driver_probe(struct i2c_client *, const struct i2c_device_id *);
static int driver_remove(struct i2c_client *);
static int driver_detect(struct i2c_client *, struct i2c_board_info *);
static const struct i2c_device_id tmp006_i2c_id[] = {{ "tmp006", 0 }, { }};
MODULE_DEVICE_TABLE(i2c, tmp006_i2c_id);
/*static struct bus_type driver_bus_type = 
{
	
};*/
static struct i2c_driver tmp006_driver =
{
	.driver = {
			.name =  "tmp006",
			.owner = THIS_MODULE,
			//.bus = 
		},
	.probe =    driver_probe,
	.remove =   driver_remove,
	.id_table = tmp006_i2c_id
};

static int driver_probe(struct i2c_client * client, const struct i2c_device_id * device_id)
{
	printk(KERN_INFO "driver_probe");

 /*struct uda1380_priv *uda1380;
802         int ret;
803 
804         uda1380 = devm_kzalloc(&i2c->dev, sizeof(struct uda1380_priv),
805                                GFP_KERNEL);
806         if (uda1380 == NULL)
807                 return -ENOMEM;
808 
809         i2c_set_clientdata(i2c, uda1380);
810         uda1380->control_data = i2c;
811 
812         ret =  snd_soc_register_codec(&i2c->dev,
813                         &soc_codec_dev_uda1380, uda1380_dai, ARRAY_SIZE(uda1380_dai));
814         return ret;*/


	return 0;
}
static int driver_remove(struct i2c_client *client)
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
}
MODULE_LICENSE("GPL");

module_init(_driver_init);
module_exit(_driver_exit);
