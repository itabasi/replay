from workflow.patterns.controlflow import IF_ELSE
from job import check, comp, job_a, job_b
 
my_workflow = [
    IF_ELSE(
        check,
        [ job_a, job_b ],
        [ job_a ]
    ),
    comp
]
