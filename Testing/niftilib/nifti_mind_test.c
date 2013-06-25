/*
 * nifti_mind_test -- this exercises a particular bug, whereby nifti_copy_nim_info was
 * not working correctly, which left the copy of an image pointing to the same
 * extension list, which could cause all sorts of grief.
 */
#include <nifti1_io.h>
#include <stdio.h>

static int rcount = 0;

nifti_image* generate_dwi_image(const char* filename, int size)
{
	int ii;
	nifti_1_header reference_header;
	memset(&reference_header,0,sizeof(reference_header));
	reference_header.sizeof_hdr=sizeof(reference_header);

	reference_header.dim[0]=5;
	reference_header.dim[1]=2;
	reference_header.dim[2]=2;
	reference_header.dim[3]=2;
	reference_header.dim[4]=1;
	reference_header.dim[5]=size;
	reference_header.dim[6]=1;
	reference_header.dim[7]=1;
	reference_header.intent_p1=10101010.101F;
	reference_header.intent_p2=987654321.0F;
	reference_header.intent_p3=-1234.0F;
	reference_header.intent_code=NIFTI_INTENT_VECTOR;
	reference_header.datatype=DT_FLOAT;
	reference_header.pixdim[0]=-1.0F; /* this is really qfac */
	reference_header.pixdim[1]=0.25F;
	reference_header.pixdim[2]=0.5F;
	reference_header.pixdim[3]=1.0F;
	reference_header.pixdim[4]=2.0F;
	reference_header.pixdim[5]=4.0F;
	reference_header.pixdim[6]=-2.0e10F;
	reference_header.pixdim[7]=-2.0e10F;
	reference_header.vox_offset=0;
	reference_header.scl_slope=0.25;
	reference_header.scl_inter=128;
	reference_header.qform_code=NIFTI_XFORM_SCANNER_ANAT;
	reference_header.quatern_b=-0.5F;
	reference_header.quatern_c= 0.5F;
	reference_header.quatern_d=-0.5F;
	reference_header.qoffset_x=reference_header.dim[1]/2.0F;
	reference_header.qoffset_y=reference_header.dim[2]/2.0F;
	reference_header.qoffset_z=reference_header.dim[3]/2.0F;
	reference_header.sform_code=NIFTI_XFORM_SCANNER_ANAT;
	reference_header.srow_x[0]=0.5;
	reference_header.srow_x[1]=0.0;
	reference_header.srow_x[2]=0.0;
	reference_header.srow_x[3]=0.0;
	reference_header.srow_y[0]=0.0;
	reference_header.srow_y[1]=1.0;
	reference_header.srow_y[2]=0.0;
	reference_header.srow_y[3]=0.0;
	reference_header.srow_z[0]=0.0;
	reference_header.srow_z[1]=0.0;
	reference_header.srow_z[2]=2.0;
	reference_header.srow_z[3]=0.0;
	reference_header.magic[0]='n';
	reference_header.magic[1]='+';
	reference_header.magic[2]='1';
	reference_header.magic[3]='\0';
	reference_header.bitpix=32;
	strncpy(reference_header.intent_name,"MiND",16);
	strncpy(reference_header.descrip,"Hello World",80);

	nifti_image * nim = nifti_convert_nhdr2nim(reference_header, filename);

	int npixel = 1;
	for(ii = 1 ; ii < 8; ii++) 
		npixel *= nim->dim[ii];

  	nim->data = malloc(npixel*sizeof(float)); 
	for(ii = 0 ; ii < npixel ; ii++) {
		int rint;
		float* rfloat = (float*)&rint;

		rint = rand();
		((float*)nim->data)[ii] = *rfloat;
	}

	return nim;
}

void generate_rawdwi_image(const char* filename, int seed)
{
	srand(seed);

	int VECTOR_SIZE = 13;
	int ii;
	
	nifti_image* nim = generate_dwi_image(filename, VECTOR_SIZE);
	NiftiRawDWI data[VECTOR_SIZE];
	
	srand(seed);
	for(ii = 0 ; ii < VECTOR_SIZE; ii++) {
		int rint;
		float* rfloat = (float*)&rint;

		rint = rand();
		data[ii].azimuth = *rfloat;
		
		rint = rand();
		data[ii].zenith = *rfloat;
		
		rint = rand();
		data[ii].bvalue = *rfloat;
	}
	
	nifti_add_mind(nim, MIND_RAWDWI, (NiftiMindExt*)data);
	fprintf(stderr, "Writing %s\n", nim->iname);
	nifti_image_write(nim);
}

void generate_tensor_image(const char* filename, int seed)
{
	srand(seed);

	int VECTOR_SIZE = 6;
	int ii;
	
	nifti_image* nim = generate_dwi_image(filename, VECTOR_SIZE);
	NiftiTensorPos data[VECTOR_SIZE];
	
	srand(seed);
	for(ii = 0 ; ii < VECTOR_SIZE; ii++) {
		data[ii].row = rand();
		data[ii].col = rand();
	}
	
	nifti_add_mind(nim, MIND_DTENSOR, (NiftiMindExt*)data);
	nifti_image_write(nim);
}

void generate_dsphere_image(const char* filename, int seed)
{
	srand(seed);

	int VECTOR_SIZE = 128;
	int ii;
	
	nifti_image* nim = generate_dwi_image(filename, VECTOR_SIZE);
	NiftiMeshPoint data[VECTOR_SIZE];
	
	srand(seed);
	for(ii = 0 ; ii < VECTOR_SIZE; ii++) {
		int rint;
		float* rfloat = (float*)&rint;

		rint = rand();
		data[ii].azimuth = *rfloat;
		
		rint = rand();
		data[ii].zenith = *rfloat;
	}
	
	nifti_add_mind(nim, MIND_DISCSPHFUNC, (NiftiMindExt*)data);
	nifti_image_write(nim);
}

void generate_hsphere_image(const char* filename, int seed)
{
	srand(seed);

	int VECTOR_SIZE = 16;
	int ii;
	
	nifti_image* nim = generate_dwi_image(filename, VECTOR_SIZE);

	NiftiSphereHarmonic data[VECTOR_SIZE];
	
	srand(seed);
	for(ii = 0 ; ii < VECTOR_SIZE; ii++) {
		data[ii].degree = rand();
		data[ii].order = rand();
	}
	
	nifti_add_mind(nim, MIND_REALSPHARMCOEFFS, (NiftiMindExt*)data);
	nifti_image_write(nim);
}

int check_image(const char* filename, int seed)
{
	/*
	 * Read Test Image and compare with generated data
	 */
	nifti_set_debug_level(2);
	nifti_image *i1 = nifti_image_read(filename, 0);
	nifti_image_infodump(i1);

	fprintf(stderr, "Valid Nifti Extensions = %i\n", valid_nifti_extensions(i1));
	fprintf(stderr, "Intent: %s\n", i1->intent_name);
	fprintf(stderr, "Num extents: %i\n", i1->num_ext);
	if(strncmp(i1->intent_name, "MiND", 5) != 0) {
		return -1;
	}

	int ii = 0;
	NiftiMindExt* out = NULL;
	NiftiMindCode res = nifti_get_mind(i1, &out);
	fprintf(stderr, "Num extents: %i\n", i1->num_ext);
	fprintf(stderr, "%i\n", res);

	srand(seed);

	switch(res) {
		case MIND_RAWDWI: 
			{
				int irand;
				float* frand = (float*)&irand;

				fprintf(stderr, "Raw DWI Image\n");
				NiftiRawDWI* arr = (NiftiRawDWI*)out;
				for(ii = 0 ; ii < i1->nu ; ii++) {
					irand = rand();
					if(arr[ii].azimuth != *frand) 
						return -3;
					
					irand = rand();
					if(arr[ii].zenith!= *frand) 
						return -4;
					
					irand = rand();
					if(arr[ii].bvalue != *frand) 
						return -2;
				}
			}
			break;
		case MIND_DTENSOR:
			{
				fprintf(stderr, "Tensor Image\n");
				NiftiTensorPos* arr = (NiftiTensorPos*)out;
				for(ii = 0 ; ii < i1->nu ; ii++) {
					if(arr[ii].row != rand()) 
						return -5;
					
					if(arr[ii].col != rand()) 
						return -6;
				}
			}
			break;
		case MIND_DISCSPHFUNC:
			{
				int irand;
				float* frand = (float*)&irand;
				fprintf(stderr, "Discrete Spherical Lattice Image\n");
				NiftiMeshPoint* arr = (NiftiMeshPoint*)out;
				for(ii = 0 ; ii < i1->nu ; ii++) {
					irand = rand();
					if(arr[ii].azimuth != *frand) 
						return -7;
					
					irand = rand();
					if(arr[ii].zenith != *frand) 
						return -8;
				}
			}
			break;
		case MIND_REALSPHARMCOEFFS:
			{
				fprintf(stderr, "Real Spherical Harmonics Image\n");
				NiftiSphereHarmonic* arr = (NiftiSphereHarmonic*)out;
				for(ii = 0 ; ii < i1->nu ; ii++) {
					if(arr[ii].degree != rand()) 
						return -9;
					
					if(arr[ii].order != rand()) 
						return -10;
				}
			}
			break;
	}

	free(out);
	nifti_image_free(i1);

	return 0;
}

int main (int argc, char *argv[])
{
	//Raw DWI
	generate_rawdwi_image("rawdwi.nii.gz", 13);
	if(check_image("rawdwi.nii.gz", 13) != 0) 
		return EXIT_FAILURE;
	
	generate_tensor_image("tensor.nii.gz", 13);
	if(check_image("tensor.nii.gz", 13) != 0)
		return EXIT_FAILURE;
	
	generate_hsphere_image("hsphere.nii.gz", 13);
	if(check_image("hsphere.nii.gz", 13) != 0) 
		return EXIT_FAILURE;
	
	generate_dsphere_image("dsphere.nii.gz", 13);
	if(check_image("dsphere.nii.gz", 13) != 0) 
		return EXIT_FAILURE;
	
	return 0;
}

