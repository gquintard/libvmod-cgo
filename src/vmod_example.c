#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "cache/cache.h"

#include "vtim.h"
#include "vcc_example_if.h"

#include "../libhelpers.h"
const size_t infosz = 64;
char	     *info;

/*
 * handle vmod internal state, vmod init/fini and/or varnish callback
 * (un)registration here.
 *
 * malloc'ing the info buffer is only indended as a demonstration, for any
 * real-world vmod, a fixed-sized buffer should be a global variable
 */

int v_matchproto_(vmod_event_f)
vmod_event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	char	   ts[VTIM_FORMAT_SIZE];
	const char *event = NULL;

	(void) ctx;
	(void) priv;

	switch (e) {
	case VCL_EVENT_LOAD:
		info = malloc(infosz);
		if (! info)
			return (-1);
		event = "loaded";
		break;
	case VCL_EVENT_WARM:
		event = "warmed";
		break;
	case VCL_EVENT_COLD:
		event = "cooled";
		break;
	case VCL_EVENT_DISCARD:
		free(info);
		return (0);
		break;
	default:
		return (0);
	}
	AN(event);
	VTIM_format(VTIM_real(), ts);
	snprintf(info, infosz, "vmod_example %s at %s", event, ts);

	return (0);
}

VCL_STRING
vmod_info(VRT_CTX)
{
	(void) ctx;

	return (info);
}

VCL_STRING
vmod_hello(VRT_CTX, VCL_STRING name)
{
	char *p;

	char *pgo = Go_hello(TRUST_ME(name));
	p = WS_Copy(ctx->ws, pgo, -1);
	free(pgo);
	return (p);
}
