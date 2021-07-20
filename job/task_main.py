from workflow.engine import GenericWorkflowEngine
from my_flow import my_workflow
 
import sys
from distutils.util import strtobool
 
class ParamWrapper(object):
    def __init__(self, data):
        self.data = data
        self.job_a = 'yet'
        self.job_b = 'yet'
 
my_engine = GenericWorkflowEngine()
my_engine.callbacks.replace(my_workflow)
 
my_object = ParamWrapper(strtobool(sys.argv[1]))
my_engine.process([my_object])
