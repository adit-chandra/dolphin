import binascii
import os
import sys
import zmq
import time

def chunk(l, n):
  return [l[i:i+n] for i in range(0, len(l), n)]

def parseMessage(message):
  lines = message.splitlines()
  
  diffs = chunk(lines, 2)
  
  for diff in diffs:
    diff[1] = binascii.unhexlify(diff[1].zfill(8))
  
  return diffs

class MemoryWatcherZMQ:
  def __init__(self):
    try:
      import zmq
    except ImportError as err:
      print("ImportError: {0}".format(err))
      sys.exit("Either install pyzmq or run with the --nozmq option")

    context = zmq.Context()

    self.socket = context.socket(zmq.REP)
    self.socket.bind("tcp://*:5555")
	
    self.messages = None
  
  def get_messages(self):
    if self.messages is None:
      message = self.socket.recv()
      message = message.decode('utf-8')
      self.messages = parseMessage(message)
    
    return self.messages
  
  def advance(self):
    self.socket.send(b'')
    self.messages = None

mw = MemoryWatcherZMQ()

while True:
    #  Wait for next request from client
    messages = mw.get_messages()
    print("Received Request")
    print(messages)
    mw.advance()

    #  Do some 'work'
    # time.sleep(1)
