/**
 * \file
 * \brief Definição das funções do Main, que é responsável por realizar o boot.
 **/
#include "../interface/KIgdt.h"
#include "../interface/KIide.h"
#include "../interface/KIidt.h"
#include "../interface/KIio.h"
#include "../interface/KIirq.h"
#include "../interface/KIisrs.h"
#include "../interface/KIkb.h"
#include "../interface/KIscrn.h"
#include "../interface/KFat.h"
#include "../../modulos/interface/IMemTable.h"
#include "../../modulos/interface/Igmem.h"
#include "../../modulos/interface/Igpro.h"
#include "../../modulos/interface/Itim.h"
#include "../../Iif.h"
#include "../../modulos/include/MultiBoot.h"

