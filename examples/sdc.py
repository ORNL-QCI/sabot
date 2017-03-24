import json
import random
import zmq

sabotAddress = 'ipc:///tmp/sabot'
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect(sabotAddress);

socket.send(json.dumps(
        {
            'method' : 'create_system',
            'parameters' : ['chp_state']
        }
    )+'\0')
systemId = json.loads(socket.recv())['result']
print('systemId: {}'.format(systemId))

circuit = 'init 2\n' \
          'h 0\n' \
          'c 0,1\n'
# Randomly encode a 00, 01, 10, 11
encodingChoices = ['i 0\n', 'x 0\n', 'z 0\n', 'y 0\n']
encoded = random.choice(encodingChoices)
print('Encoded: {0:02}'.format(int(bin(encodingChoices.index(encoded))[2:])))
circuit += encoded
circuit += 'c 0,1\n' \
           'h 0\n' \
           'm 0\n' \
           'm 1\n'
socket.send(json.dumps(
        {
            'method' : 'compute_result',
            'parameters' : [systemId,
                            'chpext',
                            circuit,
                            ord('\n')]
        }
    )+'\0')
computeResultResult = json.loads(socket.recv())['result']

print('Measurement result: {}'.format(computeResultResult))


socket.send(json.dumps(
        {
            'method' : 'delete_system',
            'parameters' : [systemId]
        }
    )+'\0')
json.loads(socket.recv())['result']
socket.disconnect(sabotAddress)


print('\nOverall circuit simulated:\n{}'.format(circuit))
