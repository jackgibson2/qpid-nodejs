{
   'targets': [
    {
      'target_name': 'helloworld',
      'sources': [ 'helloworld.cpp' ],
      'conditions': [
          ['OS=="linux"', {
            'ldflags': [
              '-lqpidmessaging',
          ],
        }],
      ],
    },
  ],
}
