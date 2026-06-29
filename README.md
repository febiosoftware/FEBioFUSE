# FEBioFUSE

The FUSE (**F**EBio **U**nified **S**imulation and **E**xchange) library is an FEBio plugin library for coupling and exchanging data between FEBio models. It allows users to couple different FEBio models and run them together, exchanging data where needed.

The models exchange data via mesh data maps. Currently, it is assumed that all models involved use the exact same mesh and data maps that exchange data are defined on the same element set.

One of the models is designated as the primary model. This model will be the model that is specified on the FEBio command line (with the `-i` command line option). All the other models are identified as secondary models.

The interaction between the models is defined via a separate input file that is described below.

## FUSE option file
The FUSE options file is an xml formatted file that defines the models involved in the coupled simulation and how data is exchanged between the models. The name of this file is chosen by the user, but the preferred extension is `.opt`. 

The root element is defined by the `febio_study` tag, which needs the `type="fuse"` attribute.

```xml
<febio_study type="fuse">
```

There are two main sections in the file:

There are three main sections in the file:

* `models`: This defines the models that are involved in the coupled simulation. 
* `exchanges`: This defines the data that needs to be exchanged between models.
* `strategy`: This defines the solution strategy of the coupled solver.

### The **models** section
The **models** section lists the secondary models that are involved in the simulation. Each model is defined via the model tag. This tag takes two attributes:

* **name**: specify a name for this model to refer to later. 
* **file**: specify the relative input file name for this model. 

Note that the primary model should not be defined in this list. By default, the name of the primary model is "fem".  To change this, you can use the `primary_model` tag. The name is set with the `name` attribute.

```xml
<models>
    <primary_model name="chem"/>
    <model name="mech" file="mech.feb" />
</models>
```

### The **exchanges** section
This section defines how data is exchanged between different models. Each exchange is defined via an `exchange` element. Each exchange identifies a source and a destination model. For a model to serve as a destination model, it must define a mesh data to receive the data in.

The **exchange** element defines two attributes. 

* **src**: the source data, evaluated on the source model. 
* **dst**: the destination data, evaluated on the destination model’s mesh data map.

In addition, this element has the following optional property. 

* **filter**: a 1D function that will be applied to the source data before mapping it to the destination.

```xml
<exchange src="mech.elem_data('effective stress')" dst="chem.mesh_data('k_map')">
    <filter type="scale">
        <scale>0.1</scale>
    </filter
</exchange>
```
### The strategy section
The `strategy` allows the user to select and configure the solution strategy for solving the coupled model. The solution strategy is chosen by specifying the strategy in the `type` attribute. 

The following strategies are supported. 

* `time-decoupled`: The time stepping of the secondary models is not coupled to the primary model. As the primary model advances in time, the secondary models are solved from start to finish when the data of secondary models is requested by the primary model. Data from the end state of the secondary model is transferred back to the primary model. 

```xml
<strategy type="time-decoupled">
    <exchange_interval>5</exchange_interval>
</strategy>
```

## Running the coupled model
To run the coupled model using the fuse library, use the following FEBio command line.

```
>febio4 -i primary.feb -task=fuse options.opt
```

Here, *primary.feb* is the name of the primary input file (this can be any FEBio input file). The `-task=fuse` option tells FEBio to use FUSE to solve the primary model. The *options.opt* is the FUSE input file (again, you can choose the name of this file). 
Along with the primary model, FUSE will run the secondary models as needed and exchange data between all models as defined by the options file. 
FEBio will output the standard log and plot files for the primary model. In addition, it will create a plot file for the secondary models. The name of the plot file is derived from the secondary model’s name.
