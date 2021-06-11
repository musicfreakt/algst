package Factory.service;

import Factory.dao.ManagerDAO;
import Factory.model.Manager;
import java.util.List;

public class ManagerService
{
    private static ManagerDAO managerDao;

    public ManagerService()
    {
        managerDao = new ManagerDAO();
    }

    public void persist(Manager entity) {
        managerDao.openCurrentSessionwithTransaction();
        managerDao.persist(entity);
        managerDao.closeCurrentSessionwithTransaction();
    }

    public void update(Manager entity) {
        managerDao.openCurrentSessionwithTransaction();
        managerDao.update(entity);
        managerDao.closeCurrentSessionwithTransaction();
    }

    public Manager findById(int id) {
        managerDao.openCurrentSession();
        Manager manager = managerDao.findById(id);
        managerDao.closeCurrentSession();
        return manager;
    }

    public void delete(int id) {
        managerDao.openCurrentSessionwithTransaction();
        Manager manager = managerDao.findById(id);
        managerDao.delete(manager);
        managerDao.closeCurrentSessionwithTransaction();
    }

    public List<Manager> findAll() {
        managerDao.openCurrentSession();
        List<Manager> managers = managerDao.findAll();
        managerDao.closeCurrentSession();
        return managers;
    }

    public void deleteAll() {
        managerDao.openCurrentSessionwithTransaction();
        managerDao.deleteAll();
        managerDao.closeCurrentSessionwithTransaction();
    }
}