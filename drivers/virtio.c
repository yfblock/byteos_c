#include <console.h>
#include <driver.h>
#include <virtio.h>

udevice_t* virtio_init(dtb_node_t *node) {
    dtb_prop_t *reg_prop = dtb_find_prop(node, "reg");
    auto virtio = (virtio_regs *)dtb_read_prop_cell(reg_prop->first_cell, 2);
    if(virtio->MagicValue != VIRTIO_MAGIC_VALUE
        | virtio->Version != VIRTIO_VERSION_1
        | virtio->DeviceID == 0) return nullptr;

    debug("virtio address = %x", virtio);

    switch (virtio->DeviceID) {
        case VIRTIO_DEVICE_ID_BLK:
            virtio_block_probe(virtio, node);
            break;
        default:
            warn("virtio device id: %x is not supported", virtio->DeviceID);
    }

    return nullptr;
}

static struct udevice_id virtio_ids[] = {
    {.compatible =  "virtio,mmio"},
    {}
};

DEFINE_DRIVER virtio_driver = {
    .ids = virtio_ids,
    .uclass = UCLASS_VIRTIO,
    .probe = virtio_init
};