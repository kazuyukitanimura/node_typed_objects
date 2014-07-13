{
  'targets': [
    {
      'target_name': 'typed_objects',
      'include_dirs': [
        'src',
        '/usr/local/Cellar/google-sparsehash/2.0.2/include',
      ],
      'sources': [
        'src/typed_objects.cc',
      ],
      'cflags': [ '-O2' ],
    },
  ],
}
