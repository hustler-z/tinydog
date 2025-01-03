// @Hustler
//
// Self-Education Only

mod sched_rr;
// mod sched_rt;

pub use self::sched_rr::SchedulerRR;
// pub use self::sched_rt::SchedulerRT;

use crate::kernel::Vcpu;

// Must Implement SchedulerTrait for inner struct(the real scheduler object)
/// Scheduler type
pub enum SchedType {
	SchedRR(SchedulerRR),
	// SchedRT(SchedulerRT),
	None,
}

/// Scheduler trait, used to define common interface for different scheduler
pub trait Scheduler {
	/// initialize the scheduler, call once when scheduler is created
	fn init(&mut self);
	/// pick the next vcpu object
	fn next(&mut self) -> Option<Vcpu>;
	/// yield current vcpu
	fn do_schedule(&mut self);
	/// put vcpu into sleep, and remove it from scheduler
	fn sleep(&mut self, vcpu: Vcpu);
	/// wake up vcpu from sleep status, remember to set_active_vcpu when it is none
	fn wakeup(&mut self, vcpu: Vcpu);
	/// yield to another cpu, only used when vcpu is newly added and want to be excuted immediately
	fn yield_to(&mut self, vcpu: Vcpu);
}

pub trait SchedulerUpdate {
	fn update(&self) -> Self;
}
