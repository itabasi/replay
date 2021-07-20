def check(obj, eng):
    print('execute check')
    return obj.data
 
def job_a(obj, eng):
    print('execute Job A')
    print('  Job A  {}'.format(obj.job_a))
    print('  Job B  {}'.format(obj.job_b))
    obj.job_a = 'done'
    return obj
 
def job_b(obj, eng):
    print('execute Job B')
    print('  Job A : {}'.format(obj.job_a))
    print('  Job B : {}'.format(obj.job_b))
    obj.job_b = 'done'
    return obj
 
def comp(obj, eng):
    print('execute comp')
    print('  input : {}'.format(obj.data))
    print('  Job A : {}'.format(obj.job_a))
    print('  Job B : {}'.format(obj.job_b))
