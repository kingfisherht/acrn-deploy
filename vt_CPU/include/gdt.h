/*
 * Definition of 16 byte TSS and LDT selectors.
 */
struct tss_64_descriptor {
		uint32_t low32_value;
		uint32_t high32_value;
		uint32_t base_addr_63_32;
		uint32_t offset_12;
} __aligned(8);


/*****************************************************************************
 *
 * BEGIN: Definition of the GDT.
 *
 * NOTE:
 * If you change the size of the GDT or rearrange the location of descriptors
 * within the GDT you must change both the defines and the C structure header.
 *
 *****************************************************************************/
struct host_gdt {
	uint64_t rsvd;

	uint64_t code_segment_descriptor;
	uint64_t data_segment_descriptor;
	struct tss_64_descriptor host_gdt_tss_descriptors;
} __aligned(8);



/*****************************************************************************
 *
 * END: Definition of the GDT.
 *
 *****************************************************************************/

/*
 * x86-64 Task State Segment (TSS) definition.
 */
struct tss_64 {
	uint32_t rsvd1;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint32_t rsvd2;
	uint32_t rsvd3;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint32_t rsvd4;
	uint32_t rsvd5;
	uint16_t rsvd6;
	uint16_t io_map_base_addr;
} __packed __aligned(16);


/*
 * x86-64 Task State Segment (TSS) definition.
 */
struct tss_64 {
	uint32_t rsvd1;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint32_t rsvd2;
	uint32_t rsvd3;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint32_t rsvd4;
	uint32_t rsvd5;
	uint16_t rsvd6;
	uint16_t io_map_base_addr;
} __packed __aligned(16);