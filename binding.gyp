{
  'targets': [
    {
      'target_name': 'node_typed_objects',
      'include_dirs': [ 'src' ],
      'sources': [
        'src/xxhash.h',
        'src/xxhash.c',
      ],
      'cflags': [ '-O2' ],
    },
  ],
}
