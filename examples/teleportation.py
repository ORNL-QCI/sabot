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


# The system always initializes the qubits to |0>
createCircuit = 'init 3\n' \
                'h 1\n' \
                'c 1,2\n'
# Randomly encode a 0 or a 1 into qubit to be teleported
createCircuit += random.choice(['i 0\n', 'x 0\n'])
socket.send(json.dumps(
        {
            'method' : 'create_state',
            'parameters' : [systemId,
                            'chpext',
                            createCircuit,
                            ord('\n')]
        }
    )+'\0')
stateId = json.loads(socket.recv())['result']
print('stateId: {}'.format(stateId))


measureCircuit = 'c 0,1\n' \
                 'h 0\n' \
                 'm 0\n' \
                 'm 1\n'
socket.send(json.dumps(
        {
            'method' : 'measure_state',
            'parameters' : [systemId,
                            stateId,
                            'chpext',
                            measureCircuit,
                            ord('\n')]
        }
    )+'\0')
result = json.loads(socket.recv())['result']
print('measurement of qubits 0 and 1: {}'.format(result))


# Correction step
finalCircuit = ''
# Controlled-X
if result[1] == '1':
    finalCircuit += 'x 2\n'
# Controlled-Z
if result[0] == '1':
    finalCircuit += 'z 2\n'
finalCircuit += 'm 2'

socket.send(json.dumps(
        {
            'method' : 'measure_state',
            'parameters' : [systemId,
                            stateId,
                            'chpext',
                            finalCircuit,
                            ord('\n')]
        }
    )+'\0')
result = json.loads(socket.recv())['result']
print('final state: {}'.format(result))


# Cleanup
socket.send(json.dumps(
        {
            'method' : 'delete_state',
            'parameters' : [systemId,
                            stateId]
        }
    )+'\0')
json.loads(socket.recv())['result']
socket.send(json.dumps(
        {
            'method' : 'delete_system',
            'parameters' : [systemId]
        }
    )+'\0')
json.loads(socket.recv())['result']
socket.disconnect(sabotAddress)


print('\nOverall circuit simulated:\n{}{}{}'.format(createCircuit,
                                                    measureCircuit,
                                                    finalCircuit))

