# Common RTL Modules

Reusable blocks shared across CPU versions.

Typical modules here:
- muxes (2:1, 4:1, parametric)
- adders / comparators
- sign/zero extension helpers
- small pipeline utilities

Goal: avoid duplication in single-cycle + pipeline designs.
