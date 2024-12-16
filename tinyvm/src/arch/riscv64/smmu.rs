use crate::device::EmuContext;
use crate::kernel::Vm;

pub fn smmu_init() {
	// TODO: implement smmu's initialization
	todo!()
}

#[allow(unused_variables)]
pub fn smmu_add_device(context_id: usize, stream_id: usize) -> bool {
	todo!()
}

#[allow(unused_variables)]
pub fn smmu_vm_init(vm: &Vm) -> bool {
	todo!()
}

#[allow(unused_variables)]
pub fn emu_smmu_handler(_emu_dev_id: usize, emu_ctx: &EmuContext) -> bool {
	todo!()
}
