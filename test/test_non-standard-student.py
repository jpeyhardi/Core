from test_distribution import AbstractTestContinuousUnivariateDistribution

from statiskit import core

import unittest
from nose.plugins.attrib import attr

@attr(linux=True,
      osx=True,
      win=True,
      level=1)
class TestNonStandardStudent(unittest.TestCase, AbstractTestContinuousUnivariateDistribution):

    @classmethod
    def setUpClass(cls):
        """Test non-standard Student distribution construction"""
        cls._dist = core.NonStandardStudentDistribution(1.,1.,5.)

    def test_moments(self):
        """Test moments of Student distribution"""
        data = self._dist.simulation(1000)
        self.assertAlmostEqual(abs(data.location-self._dist.mean), self._epsilon, delta=self._delta)
        self.assertAlmostEqual(abs(data.dispersion - self._dist.variance) / data.dispersion, self._epsilon, delta=self._delta)

    @classmethod
    def tearDownClass(cls):
        """Test non-standard Student distribution deletion"""
        del cls._dist