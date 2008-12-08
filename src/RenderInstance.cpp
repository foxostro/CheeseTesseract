#include "stdafx.h"
#include "RenderInstance.h"
#include "RenderMethodTags.h"

RenderInstance::RenderInstance() {
	canidateForShadowReceiver = false;
	canidateForShadowCaster = false;
	
#if 0
	// FFP fall-back method, specifically
	specificRenderMethod = METHOD_DIFFUSE_TEX_FFP;
	metaRenderMethod = TAG_UNSPECIFIED;
#elif 0
	// display object normals
	specificRenderMethod = METHOD_CG_RENDER_NORMALS;
	metaRenderMethod = TAG_UNSPECIFIED;
#else
	// initially specify the highest quality diffuse texture shader this
	// machine can provide
	specificRenderMethod = METHOD_UNSPECIFIED;
	metaRenderMethod = TAG_DIFFUSE_TEX;
#endif
}
