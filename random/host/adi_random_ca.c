// Copyright 2023, Analog Devices, Inc.
//
// SPDX-License-Identifier: BSD-3-Clause

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/param.h>
#include <tee_client_api.h>
#include <adi_random_ta.h>

static struct option options[] = {
	{ "get-random",	no_argument,		0, 'r' },
	{ "help",		no_argument,		0, 'h' },
	{ 0,			0,			0, 0   }
};
static const char *shortopts = ":r:h";

static char *optarghelp[] = {
	"--get-random         ",
	"--help               ",
};

static char *opthelp[] = {
	"number of bytes to get random number (16 bytes as default)",
	"display this help text"
};

/*
 * print_usage
 */
static void
print_usage (void)
{
	int i;
	fprintf(stdout, "\nUsage:\n");
	fprintf(stdout, "\trandom-gen\n");
	fprintf(stdout, "Options:\n");
	for (i = 0; i < sizeof(options)/sizeof(options[0]) && options[i].name != 0; i++) {
		fprintf(stdout, " %s\t%c%c\t%s\n",
		       optarghelp[i],
		       (options[i].val == 0 ? ' ' : '-'),
		       (options[i].val == 0 ? ' ' : options[i].val),
		       opthelp[i]);
	}

} /* print_usage */

/*
 * dump_random_number
 */
static void 
dump_random_number(const uint8_t *buf, size_t len)
{
	const uint8_t *addr = buf;
	size_t count;
	size_t i;

	for (count = 0 ; count < len; count += 16) {
		for (i = 0; i < MIN(len - count, 16); i++)
			fprintf(stdout, "%02hhx", *(addr + i));
		fprintf(stdout, "\n");
		addr += 16;
	}
} /* dump_random_number */

int main(int argc, char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op = { 0 };
	TEEC_UUID uuid = ADI_TA_RANDOM_UUID;
	int rng_size = 16;
	int c, which;
	uint8_t *rng_buff;
	uint32_t err_origin;

	while ((c = getopt_long_only(argc, argv, shortopts, options, &which)) != -1) {

		switch (c) {
		case 'h':
			print_usage();
			return 0;
		case 'r':
			rng_size = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Error: unrecognized option\n");
			print_usage();
			return 1;
		} /* switch (c) */

	} /* while getopt */

	if (rng_size <= 0)
		return 1;	

	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS) {
		fprintf(stderr, "TEEC_InitializeContext failed with code 0x%x\n", res);
		return 1;
	}

	res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS) {
		fprintf(stderr, "TEEC_Opensession failed with code 0x%x origin 0x%x\n", res, err_origin);
		TEEC_FinalizeContext(&ctx);
		return 1;
	}

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));

    rng_buff = calloc(1, rng_size);
	if (rng_buff == NULL) {
		fprintf(stderr, "calloc failed.\n");
		TEEC_CloseSession(&sess);
		TEEC_FinalizeContext(&ctx);
		return 1;
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = rng_buff;
	op.params[0].tmpref.size = rng_size;

	fprintf(stdout, "Invoking TA to generate random number...\n");
	res = TEEC_InvokeCommand(&sess, TA_RANDOM_CMD_GENERATE, &op, &err_origin);
	if (res != TEEC_SUCCESS) {
		fprintf(stderr, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x\n",	res, err_origin);
		TEEC_CloseSession(&sess);
		TEEC_FinalizeContext(&ctx);
		return 1;
	}

	fprintf(stdout, "TA generated random value...\n");
	dump_random_number(rng_buff, rng_size);

	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);

	return 0;
}
