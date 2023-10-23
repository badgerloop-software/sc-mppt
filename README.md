# sc-mppt
Repository for MPPT PCB specific application and driver code

### PID logic
The current logic for the PID loop uses the basic s-domain function, $u(t)=K_p+K_i\cdot\frac{1}{s}+K_d\cdot s$, converted into the discrete-time domain using a bilinear transform. Including derivative damping, this gives equations for each of the terms at timestep $n$ as follows:

$$P_n=K_p\cdot e_n$$

$$I_n=\frac{K_iT}{2}(e_n+e_{n-1})+I_{n-1}$$

$$D_n=\frac{2K_d}{2\tau_D+T}(e_n-e_{n-1})+\frac{2\tau_D - T}{2\tau_D+T}\cdot D_{n-1}$$

where $e$ represents the error, $T$ is the size of the timestep, and $\tau_D$ is the derivative damping term.
The integral term may need to be clamped as a countermeasure against integral windup.

### MPPT optimization
Instead of using the Perturb and Observe (P&O) method used by the previous iteration of the MPPT controller, we will use a Incremental Conductance method, as incremental conductance operates better under variable weather conditions.
P&O checks power values for a slightly higher voltage and slightly lower voltage, and "perturbs" the voltage in the direction that increases the power. This method is prone to getting stuck on local maxima, and as such performs very poorly
under fluctatuing weather conditions since the global maxima is constantly shifting. Incremental conductance checks the conductance values, and uses the fact that $\frac{\mathrm{d}I}{\mathrm{d}V} + \frac{I}{V} = 0$ is the maximum power point. That is,

$\frac{\mathrm{d}I}{\mathrm{d}V}+\frac{I}{V}=0$, at MPP

$\frac{\mathrm{d}I}{\mathrm{d}V}+\frac{I}{V}>0$, left of MPP

$\frac{\mathrm{d}I}{\mathrm{d}V}+\frac{I}{V}<0$, right of MPP

For further consideration is to apply Bayesian optimization, which will be more robust than just using incremental conductance. [Relevant reading](https://link.springer.com/article/10.1007/s42452-021-04538-z).
