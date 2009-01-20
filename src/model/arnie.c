#include "arnie.h"
#include "model.h"
#include "arnie_params.h"
#include <math.h>

//#define M_PI_2     1.5707963267948966192
#define M_PI_5_6th 2.6179938779914944

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

struct ar_m_limb ar_m_ll_j1 = { NULL, &ar_m_ll_j2,
				{ AR_M_DH_ll_j1_theta_off,
				  AR_M_DH_ll_j1_alpha,
				  AR_M_DH_ll_j1_d,
				  AR_M_DH_ll_j1_a },
				AR_M_DH_ll_j1_theta_min,
				AR_M_DH_ll_j1_theta_max
};
    
struct ar_m_limb ar_m_ll_j2 = { &ar_m_ll_j1, &ar_m_ll_j3,
				{ AR_M_DH_ll_j2_theta_off, 
				  AR_M_DH_ll_j2_alpha, 
				  AR_M_DH_ll_j2_d, 
				  AR_M_DH_ll_j2_a },
				AR_M_DH_ll_j2_theta_min,
				AR_M_DH_ll_j2_theta_max
};

struct ar_m_limb ar_m_ll_j3 = { &ar_m_ll_j2, &ar_m_ll_j4,
				{ AR_M_DH_ll_j3_theta_off, 
				  AR_M_DH_ll_j3_alpha, 
				  AR_M_DH_ll_j3_d, 
				  AR_M_DH_ll_j3_a },
				AR_M_DH_ll_j3_theta_min,
				AR_M_DH_ll_j3_theta_max
};

struct ar_m_limb ar_m_ll_j4 = { &ar_m_ll_j3, &ar_m_ll_j5,
				{ AR_M_DH_ll_j4_theta_off, 
				  AR_M_DH_ll_j4_alpha, 
				  AR_M_DH_ll_j4_d, 
				  AR_M_DH_ll_j4_a },
				AR_M_DH_ll_j4_theta_min,
				AR_M_DH_ll_j4_theta_max
};

struct ar_m_limb ar_m_ll_j5 = { &ar_m_ll_j4, &ar_m_ll_j6,
				{ AR_M_DH_ll_j5_theta_off, 
				  AR_M_DH_ll_j5_alpha, 
				  AR_M_DH_ll_j5_d, 
				  AR_M_DH_ll_j5_a },
				AR_M_DH_ll_j5_theta_min,
				AR_M_DH_ll_j5_theta_max
};

struct ar_m_limb ar_m_ll_j6 = { &ar_m_ll_j5, &ar_m_ll_j7,
				{ AR_M_DH_ll_j6_theta_off, 
				  AR_M_DH_ll_j6_alpha, 
				  AR_M_DH_ll_j6_d, 
				  AR_M_DH_ll_j6_a },
				AR_M_DH_ll_j6_theta_min,
				AR_M_DH_ll_j6_theta_max
};

struct ar_m_limb ar_m_ll_j7 = { &ar_m_ll_j6, NULL,
				{ AR_M_DH_ll_j7_theta_off, 
				  AR_M_DH_ll_j7_alpha, 
				  AR_M_DH_ll_j7_d, 
				  AR_M_DH_ll_j7_a },
				AR_M_DH_ll_j7_theta_min,
				AR_M_DH_ll_j7_theta_max
};



struct ar_m_limb ar_m_rl_j1 = { NULL, &ar_m_rl_j2,
				{ AR_M_DH_rl_j1_theta_off,
				  AR_M_DH_rl_j1_alpha,
				  AR_M_DH_rl_j1_d,
				  AR_M_DH_rl_j1_a },
				AR_M_DH_rl_j1_theta_min,
				AR_M_DH_rl_j1_theta_max
};
    
struct ar_m_limb ar_m_rl_j2 = { &ar_m_rl_j1, &ar_m_rl_j3,
				{ AR_M_DH_rl_j2_theta_off, 
				  AR_M_DH_rl_j2_alpha, 
				  AR_M_DH_rl_j2_d, 
				  AR_M_DH_rl_j2_a },
				AR_M_DH_rl_j2_theta_min,
				AR_M_DH_rl_j2_theta_max
};

struct ar_m_limb ar_m_rl_j3 = { &ar_m_rl_j2, &ar_m_rl_j4,
				{ AR_M_DH_rl_j3_theta_off, 
				  AR_M_DH_rl_j3_alpha, 
				  AR_M_DH_rl_j3_d, 
				  AR_M_DH_rl_j3_a },
				AR_M_DH_rl_j3_theta_min,
				AR_M_DH_rl_j3_theta_max
};

struct ar_m_limb ar_m_rl_j4 = { &ar_m_rl_j3, &ar_m_rl_j5,
				{ AR_M_DH_rl_j4_theta_off, 
				  AR_M_DH_rl_j4_alpha, 
				  AR_M_DH_rl_j4_d, 
				  AR_M_DH_rl_j4_a },
				AR_M_DH_rl_j4_theta_min,
				AR_M_DH_rl_j4_theta_max
};

struct ar_m_limb ar_m_rl_j5 = { &ar_m_rl_j4, &ar_m_rl_j6,
				{ AR_M_DH_rl_j5_theta_off, 
				  AR_M_DH_rl_j5_alpha, 
				  AR_M_DH_rl_j5_d, 
				  AR_M_DH_rl_j5_a },
				AR_M_DH_rl_j5_theta_min,
				AR_M_DH_rl_j5_theta_max
};

struct ar_m_limb ar_m_rl_j6 = { &ar_m_rl_j5, &ar_m_rl_j7,
				{ AR_M_DH_rl_j6_theta_off, 
				  AR_M_DH_rl_j6_alpha, 
				  AR_M_DH_rl_j6_d, 
				  AR_M_DH_rl_j6_a },
				AR_M_DH_rl_j6_theta_min,
				AR_M_DH_rl_j6_theta_max
};

struct ar_m_limb ar_m_rl_j7 = { &ar_m_rl_j6, NULL,
				{ AR_M_DH_rl_j7_theta_off, 
				  AR_M_DH_rl_j7_alpha, 
				  AR_M_DH_rl_j7_d, 
				  AR_M_DH_rl_j7_a },
				AR_M_DH_rl_j7_theta_min,
				AR_M_DH_rl_j7_theta_max
};
