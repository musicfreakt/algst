package Factory.service;

import Factory.dao.EmployeeDAO;
import Factory.model.*;
import java.util.List;

public class EmployeeService
{
    private static EmployeeDAO employeeDao;

    public EmployeeService()
    {
        employeeDao = new EmployeeDAO();
    }

    public void persist(Employee entity) {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.persist(entity);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public void update(Employee entity) {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.update(entity);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public Employee findById(int id) {
        employeeDao.openCurrentSession();
        Employee manager = employeeDao.findById(id);
        employeeDao.closeCurrentSession();
        return manager;
    }

    public void delete(int id) {
        employeeDao.openCurrentSessionwithTransaction();
        Employee manager = employeeDao.findById(id);
        employeeDao.delete(manager);
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public List<Employee> findAll() {
        employeeDao.openCurrentSession();
        List<Employee> managers = employeeDao.findAll();
        employeeDao.closeCurrentSession();
        return managers;
    }

    public void deleteAll() {
        employeeDao.openCurrentSessionwithTransaction();
        employeeDao.deleteAll();
        employeeDao.closeCurrentSessionwithTransaction();
    }

    public EmployeeDAO employeeDao()
    {
        return employeeDao;
    }
}
