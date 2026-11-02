"""Adversarial checks for evidence classification, not game behavior fixtures."""
import unittest
from cross_version_exact_spans import exact_offsets

BODY = bytes(range(32, 72))

class EvidenceCases(unittest.TestCase):
    def test_unique_complete_span(self):
        self.assertEqual(exact_offsets(b'\xff' * 8 + BODY + b'\xff' * 8, BODY, 0x8c010000)['offsets'], [8])

    def test_matching_prefix_does_not_hide_changed_tail_constant(self):
        changed = BODY[:-2] + b'\x00\x00'
        self.assertEqual(exact_offsets(changed, BODY, 0x8c010000)['offsets'], [])

    def test_duplicate_body_remains_ambiguous(self):
        result = exact_offsets(BODY + b'\xff' * 8 + BODY, BODY, 0x8c010000)
        self.assertEqual(result['offsets'], [0, 48])
        self.assertFalse(result['truncated'])

    def test_halfword_alignment_change_is_not_accepted(self):
        self.assertEqual(exact_offsets(b'\xff\xff' + BODY, BODY, 0x8c010000)['offsets'], [])
        self.assertEqual(exact_offsets(b'\xff\xff' + BODY, BODY, 0x8c010002)['offsets'], [2])

    def test_absolute_address_changes_are_not_masked(self):
        old = BODY[:12] + bytes.fromhex('004e758c') + BODY[16:]
        new = BODY[:12] + bytes.fromhex('004e768c') + BODY[16:]
        self.assertEqual(exact_offsets(new, old, 0x8c010000)['offsets'], [])

    def test_short_repeated_returns_do_not_become_routine_matches(self):
        self.assertEqual(exact_offsets(b'\x0b\x00\x09\x00' * 16, b'\x0b\x00\x09\x00', 0x8c010000)['excluded'],
                         'span_shorter_than_minimum')

    def test_excess_candidates_are_marked_truncated(self):
        result = exact_offsets(BODY * 4, BODY, 0x8c010000, limit=2)
        self.assertEqual(result['offsets'], [0, 40])
        self.assertTrue(result['truncated'])

    def test_invalid_instruction_boundaries_are_rejected(self):
        with self.assertRaises(ValueError):
            exact_offsets(BODY, BODY, 0x8c010001)
        with self.assertRaises(ValueError):
            exact_offsets(BODY, BODY + b'\xff', 0x8c010000)

if __name__ == '__main__':
    unittest.main(verbosity=2)
