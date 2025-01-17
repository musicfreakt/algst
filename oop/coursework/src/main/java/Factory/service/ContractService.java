package Factory.service;

import Factory.dao.ContractDAO;
import Factory.model.*;

import java.util.Date;
import java.util.List;

public class ContractService
{
    private static ContractDAO contractDao;

    public ContractService()
    {
        contractDao = new ContractDAO();
    }

    public void persist(Contract entity) {
        contractDao.openCurrentSessionwithTransaction();
        contractDao.persist(entity);
        contractDao.closeCurrentSessionwithTransaction();
    }

    public void update(Contract entity) {
        contractDao.openCurrentSessionwithTransaction();
        contractDao.update(entity);
        contractDao.closeCurrentSessionwithTransaction();
    }

    public void setEnd(int id)
    {
        contractDao.openCurrentSessionwithTransaction();
        Contract contract = contractDao.findById(id);
        contract.setIsEnd(true);
        contractDao.update(contract);
        contractDao.closeCurrentSessionwithTransaction();
    }

    public Contract findById(int id) {
        contractDao.openCurrentSession();
        Contract contract = contractDao.findById(id);
        contractDao.closeCurrentSession();
        return contract;
    }

    public void delete(int id) {
        contractDao.openCurrentSessionwithTransaction();
        Contract contract = contractDao.findById(id);
        contractDao.delete(contract);
        contractDao.closeCurrentSessionwithTransaction();
    }

    public List<Contract> findAll()
    {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findAll();
        contractDao.closeCurrentSession();
        return contracts;
    }

    public int findNew(Date date)
    {
        contractDao.openCurrentSession();
        int num = contractDao.findNew(date);
        contractDao.closeCurrentSession();
        return num;
    }

    public int findFinish(Date date)
    {
        contractDao.openCurrentSession();
        int num = contractDao.findFinish(date).size();
        contractDao.closeCurrentSession();
        return num;
    }

    public float findFinishIncome(Date date)
    {
        contractDao.openCurrentSession();
        float income = 0;
        for (Contract a : contractDao.findFinish(date))
            income += a.getPrice();
        contractDao.closeCurrentSession();
        return income;
    }

    public List<Contract> findOutdated() {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findOutdated();
        contractDao.closeCurrentSession();
        return contracts;
    }

    public List<Contract> findByClientId(int id)
    {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findByClientId(id);
        contractDao.closeCurrentSession();
        return contracts;
    }

    public List<Contract> findOutdated(int id)
    {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findOutdated(id);
        contractDao.closeCurrentSession();
        return contracts;
    }

    public List<Contract> findTimePeriod(Date btime, Date etime)
    {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findTimePeriod(btime, etime);
        contractDao.closeCurrentSession();
        return contracts;
    }

    public List<Contract> findTimePeriod(Date btime, Date etime, int id)
    {
        contractDao.openCurrentSession();
        List<Contract> contracts = contractDao.findTimePeriod(btime, etime, id);
        contractDao.closeCurrentSession();
        return contracts;
    }

    public void deleteAll() {
        contractDao.openCurrentSessionwithTransaction();
        contractDao.deleteAll();
        contractDao.closeCurrentSessionwithTransaction();
    }

}
