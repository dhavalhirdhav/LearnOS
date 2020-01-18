void dummy_test_entrypoint()
{
}

void __main() {
	char* video_memory = (char*) 0xb8000;
	*video_memory = 'X';
}