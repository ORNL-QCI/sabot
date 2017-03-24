import json
import zmq

sabotAddress = 'ipc:///tmp/sabot'
context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect(sabotAddress)

socket.send(json.dumps(
        {
            'method' : 'create_system',
            'parameters' : ['chp_state']
        }
    )+'\0')
systemId = json.loads(socket.recv())['result']
print('systemId: {}'.format(systemId))

qubitCount = 5000
print('Creating {} qubits'.format(qubitCount))
circuit = 'init {}\n'.format(qubitCount)
for i in range(0, qubitCount):
    circuit += 'h {}\nm {}\n'.format(str(i), str(i))
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
average = sum(ord(char)-48 for char in computeResultResult)/float(qubitCount)

print('Average: {:04f} '.format(average))

socket.send(json.dumps(
        {
            'method' : 'delete_system',
            'parameters' : [systemId]
        }
    )+'\0')
json.loads(socket.recv())['result']
socket.disconnect(sabotAddress)
