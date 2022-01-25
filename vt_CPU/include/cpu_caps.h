#define	FEATURE_WORDS		15U
#define	FEAT_1_ECX		0U     /* CPUID[1].ECX */
#define	FEAT_1_EDX		1U     /* CPUID[1].EDX */
struct vcpuinfo_x86 {
	/* SDM 2-2 Vol.4 Table 2-1 uses DisplayFamily_DisplayModel to
	 * distinguish Processor Families/Processor Number Series.
	 */
	uint8_t displayfamily, displaymodel;
	uint8_t virt_bits;
	uint8_t phys_bits;
	uint32_t cpuid_level;
	uint32_t extended_cpuid_level;
	uint64_t physical_address_mask;
	uint32_t cpuid_leaves[FEATURE_WORDS];
	char model_name[64];
};

void init_pcpu_capabilities(void);
void init_pcpu_model_name(void);
void load_pcpu_state_data(void);