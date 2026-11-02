/* Authored introspection of native runtime records, never a disc-file format. */
#ifndef JP_TASK_RUNTIME_LAYOUT_H
#define JP_TASK_RUNTIME_LAYOUT_H
struct jp_task_runtime_layout {
    unsigned version, pointer_bytes, scalar_bytes;
    unsigned task_bytes, work_bytes, motion_bytes, force_bytes;
    unsigned event_bytes, collision_bytes, event_motion_bytes, event_path_bytes;
    unsigned task_spec_bytes, heap_header_bytes;
    unsigned work_vars, work_ang, work_pos, work_collision, work_event;
    unsigned event_motions, event_previous, event_paths;
    unsigned event_motion_next, event_path_next, collision_flags, collision_shapes;
};
#endif
