{
  'targets': [
    {
      'target_name': 'typed_objects',
      'include_dirs': [ 'src' ],
      'sources': [
        'src/typed_objects.cc',
        'src/xxhash.h',
        'src/xxhash.c',
      ],
      'cflags': [ '-O2' ],
    },
  ],
}
