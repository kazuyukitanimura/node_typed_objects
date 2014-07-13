{
  'targets': [
    {
      'target_name': 'typed_objects',
      'include_dirs': [
        'src',
      ],
      'sources': [
        'src/typed_objects.cc',
      ],
      'cflags': [
        '-Ofast',
      ],
      'conditions': [
        [ 'OS=="mac"',
          {
            'xcode_settings': {
              'OTHER_CPLUSPLUSFLAGS' : [
                '-std=gnu++11',
                '-stdlib=libc++',
                '-I/usr/local/Cellar/google-sparsehash/2.0.2/include',
              ],
              'MACOSX_DEPLOYMENT_TARGET': '10.9',
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            },
          },
        ],
      ],
    },
  ],
}
